#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "PanelConsole.h"
#include "glmath.h"
#include <list>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"

#include"Brofiler/Brofiler.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

//----------------- ModuleImporter -----------------//

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module (app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{

	BROFILER_CATEGORY("Init_ModuleImporter", Profiler::Color::Crimson);

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImporter::Start()
{
	BROFILER_CATEGORY("Start_ModuleImporter", Profiler::Color::LimeGreen);

	bool ret = true;

	return ret;
}

bool ModuleImporter::ImportFBX(char* path_fbx, char* path_texture)
{

	BROFILER_CATEGORY("ImportFBX_ModuleImporter", Profiler::Color::Yellow);

	bool ret = true;

	const aiScene* scene = aiImportFile(path_fbx, aiProcessPreset_TargetRealtime_MaxQuality);
	const aiNode* node = scene->mRootNode;

	RecursiveChild(node, path_fbx, scene, path_texture);

	aiReleaseImport(scene);

	return ret;
}

void ModuleImporter::RecursiveChild(const aiNode * node, char * path_fbx, const aiScene * scene, char * path_texture)
{
	BROFILER_CATEGORY("Recursive_ModuleImporter", Profiler::Color::LightGoldenRodYellow);

	for (uint node_num = 0; node_num < node->mNumChildren; node_num++)
	{
		RecursiveChild(node->mChildren[node_num], path_fbx, scene, path_texture);
	}
	CreateObject(node, path_fbx, scene, path_texture);
}

void ModuleImporter::CreateObject(const aiNode * node, char * path_fbx, const aiScene * scene, char * path_texture)
{
	BROFILER_CATEGORY("CreateObject_ModuleImporter", Profiler::Color::Brown);

	if (node->mNumMeshes > 0)
	{
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			//Load Position
			GameObject* aux_obj = new GameObject();
			std::string name_obj = path_fbx + std::to_string(App->viewport->root_object->children.size());
			aux_obj->SetName(name_obj.c_str());

			ComponentTransform* trans = (ComponentTransform*)aux_obj->CreateComponent(COMPONENT_TRANSFORM);
			trans->local_matrix.Set(
				node->mTransformation.a1, node->mTransformation.b1, node->mTransformation.c1, node->mTransformation.d1,
				node->mTransformation.a2, node->mTransformation.b2, node->mTransformation.c1, node->mTransformation.d2,
				node->mTransformation.a3, node->mTransformation.b3, node->mTransformation.c1, node->mTransformation.d3,
				node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c1, node->mTransformation.d4
			);

			/*trans->position = trans->global_matrix * float3(0, 0, 1);*/
			/*aiVector3D translation, scaling;
			aiQuaternion rotation;
			node->mTransformation.Decompose(scaling, rotation, translation);
			float3 pos(translation.x, translation.y, translation.z);
			float3 scale(scaling.x, scaling.y, scaling.z);
			Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			
			trans->position[0] = pos.x;
			trans->position[1] = pos.y;
			trans->position[2] = pos.z;
			trans->scale[0] = scale.x;
			trans->scale[1] = scale.y;
			trans->scale[2] = scale.z;
			trans->rotation = rot;*/

			//Load Mesh 
			ComponentMesh* m = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
			const aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];

			m->num_vertices = aimesh->mNumVertices;
			m->vertices = new float[m->num_vertices * 3];
			memcpy(m->vertices, aimesh->mVertices, sizeof(float) * m->num_vertices * 3);
			LOG("New mesh with %d vertices", m->num_vertices);

			if (aimesh->HasFaces())
			{
				m->num_index = aimesh->mNumFaces * 3;
				m->index = new uint[m->num_index]; // assume each face is a triangle

				for (uint j = 0; j < aimesh->mNumFaces; ++j)
				{
					if (aimesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m->index[j * 3], aimesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//Load Normals
			if (aimesh->HasNormals())
			{
				m->normals = new float3[aimesh->mNumVertices];
				memcpy(m->normals, aimesh->mNormals, sizeof(aiVector3D) * m->num_vertices);

				for (uint i = 0; i < m->num_index; i += 3)
				{
					uint index = m->index[i];
					vec3 vertex0(m->vertices[index * 3], m->vertices[index * 3 + 1], m->vertices[index * 3 + 2]);

					index = m->index[i + 1];
					vec3 vertex1(m->vertices[index * 3], m->vertices[index * 3 + 1], m->vertices[index * 3 + 2]);

					index = m->index[i + 2];
					vec3 vertex2(m->vertices[index * 3], m->vertices[index * 3 + 1], m->vertices[index * 3 + 2]);
					CalculateNormalTriangle(m, vertex0, vertex1, vertex2);
				}
			}

			//Load UVs
			if (aimesh->HasTextureCoords(0))
			{
				m->UV_num = aimesh->mNumUVComponents[0];
				m->UV_coord = new float[m->num_vertices * m->UV_num];

				for (uint i = 0; i < m->num_vertices; i++)
				{
					memcpy(&m->UV_coord[i * m->UV_num], &aimesh->mTextureCoords[0][i], sizeof(float) * m->UV_num);
				}

			}

			m->GenerateMesh();

			if (aimesh->HasTextureCoords(0) && path_texture != "")
			{
				ComponentTexture* texture = (ComponentTexture*)aux_obj->CreateComponent(COMPONENT_TEXTURE);
				texture->LoadTexture(path_texture);
				m->image_id = texture->image_id;
			}
			LOG("Loaded mesh file succesfully!");
			App->viewport->AddGameObject(aux_obj, OBJECT_FBX, true);

			if (aux_obj->GetParent() != App->viewport->root_object)
				trans->global_matrix = trans->local_matrix * aux_obj->GetParent()->GetComponentTransform()->global_matrix;
			else
				trans->global_matrix = trans->local_matrix;
		}

	}
}

void ModuleImporter::CalculateNormalTriangle(ComponentMesh * m, vec3 triangle_p1, vec3 triangle_p2, vec3 triangle_p3)
{
	//Calculate center of the triangle
	vec3 center = (triangle_p1 + triangle_p2 + triangle_p3) / 3;

	vec3 vec_v = triangle_p1 - triangle_p3;
	vec3 vec_w = triangle_p2 - triangle_p3;

	vec3 norm_v = cross(vec_v, vec_w);
	norm_v = normalize(norm_v);

	m->normals_face.push_back(float3(center.x, center.y, center.z));
	m->normals_face.push_back(float3(norm_v.x, norm_v.y, norm_v.z));

}

bool ModuleImporter::ImportTexture(char * path_texture)
{
	BROFILER_CATEGORY("ImportTexture_ModuleImporter", Profiler::Color::Gray);

	GameObject* object = nullptr;

	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		if ((*it_obj)->selected)
			object = (*it_obj);

	}

	if (object) {
		ComponentTexture* texture = object->GetComponentTexture();

		if (!texture)
			texture = (ComponentTexture*)object->CreateComponent(COMPONENT_TEXTURE);

		texture->LoadTexture(path_texture);
		object->GetComponentMesh()->image_id = texture->image_id;
	}
	else
	{
		C_WARNING("Warning! Object no selected. Please, select an object.");
	}
	return false;
}


bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImporter::CreateShape(shape_type type, uint sl, uint st)
{
	BROFILER_CATEGORY("CreateShapes_ModuleImporter", Profiler::Color::Orange);

	GameObject* obj = new GameObject();
	obj->SetType(OBJECT_PARSHAPE);
	par_shapes_mesh_s* shape = nullptr;

	//Create shape
	switch (type)
	{
	case SHAPE_CUBE:
		shape = par_shapes_create_cube();
		break;
	case SHAPE_SPHERE:
		shape = par_shapes_create_parametric_sphere(sl, st);
		break;
	case SHAPE_CYLINDER:
		shape = par_shapes_create_cylinder(sl, st);
		break;
	case SHAPE_CONE:
		shape = par_shapes_create_cone(sl, st);
		break;
	case SHAPE_PLANE:
		shape = par_shapes_create_plane(sl, st);
		break;
	default:
		LOG("Shape type incorrect or inexistent!");
		break;
	}

	if (!shape || !obj)
		return;

	//Create Component Transform
	ComponentTransform* trans = (ComponentTransform*)obj->CreateComponent(COMPONENT_TRANSFORM);
	trans->position[0] = 0;
	trans->position[1] = 0;
	trans->position[2] = 0;

	par_shapes_scale(shape, 1.0f, 1.0f, 1.0f);
	trans->scale[0] = 1;
	trans->scale[1] = 1;
	trans->scale[2] = 1;

	trans->rotation.x = 0;
	trans->rotation.y = 0;
	trans->rotation.z = 0;

	//Create Component Mesh
	ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(COMPONENT_MESH);
	mesh->vertices = shape->points;
	mesh->num_vertices = shape->npoints;
	mesh->index = (uint*)shape->triangles;
	mesh->num_index = shape->ntriangles * 3;
	mesh->par_shape = true;
	mesh->UV_coord = shape->tcoords;
	mesh->UV_num = 2;

	mesh->GenerateMesh();

	//Create Component Texture
	ComponentTexture* tex = (ComponentTexture*)obj->CreateComponent(COMPONENT_TEXTURE);

	obj->SetName(std::string("GameObject " + std::to_string(App->viewport->shape_num)).c_str());

	App->viewport->AddGameObject(obj);
	App->viewport->shape_num++;

	par_shapes_free_mesh(shape);
}

void LogCallback(const char* text, char* data)
{
	std::string temp_string = text;
	std::size_t pos = temp_string.find(",");
	std::string temp_string2 = temp_string.substr(0,4);
	temp_string.erase(std::remove(temp_string.begin(), temp_string.end(), '%'), temp_string.end());
	if (temp_string2 == "Info")
	{
		temp_string2 = temp_string.substr(pos + 1);
		C_INFO(temp_string2.c_str());
	}
	else if (temp_string2 == "Warn")
	{
		temp_string2 = temp_string.substr(pos + 1);
		C_WARNING(temp_string2.c_str());
	}
}