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

	//Create gameObject that contains FBX parts

	GameObject* fbx_obj = App->viewport->CreateGameObject(path_fbx);
	if (scene && scene->HasMeshes())
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			LoadNode(scene->mRootNode->mChildren[i], scene, path_texture, fbx_obj);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading FBX with path: %s", path_fbx);
	}

	return ret;
}

void ModuleImporter::LoadNode(const aiNode * node, const aiScene * scene, char * path_texture, GameObject* parent)
{
	BROFILER_CATEGORY("LoadNode_ModuleImporter", Profiler::Color::LightGoldenRodYellow);

	//Get Component transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	//Create aux_obj
	GameObject* aux_obj = App->viewport->CreateGameObject(node->mName.C_Str(), parent, pos, scale / 100, rot);

	if (node->mNumMeshes > 0)
	{
		//Load Mesh 
		ComponentMesh* m = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
		const aiMesh* aimesh = scene->mMeshes[node->mMeshes[0]];

		m->transform = aux_obj->GetComponentTransform();

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
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, path_texture, aux_obj);
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

	GameObject* obj;
	std::string name;
	par_shapes_mesh_s* shape = nullptr;

	//Create shape
	switch (type)
	{
	case SHAPE_CUBE:
		shape = par_shapes_create_cube();
		name = "Cube ";
		break;
	case SHAPE_SPHERE:
		shape = par_shapes_create_parametric_sphere(sl, st);
		name = "Sphere ";
		break;
	case SHAPE_CYLINDER:
		shape = par_shapes_create_cylinder(sl, st);
		name = "Cylinder";
		break;
	case SHAPE_CONE:
		shape = par_shapes_create_cone(sl, st);
		name = "Cone";
		break;
	case SHAPE_PLANE:
		shape = par_shapes_create_plane(sl, st);
		name = "Plane";
		break;
	default:
		LOG("Shape type incorrect or inexistent!");
		break;
	}

	name += std::to_string(App->viewport->shape_num);
	obj = App->viewport->CreateGameObject(name.c_str());
	obj->SetType(OBJECT_PARSHAPE);

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