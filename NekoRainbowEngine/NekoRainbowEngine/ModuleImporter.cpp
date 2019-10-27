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

//-------------- Devil --------------
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

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

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

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
	const aiNode* node;

	if (scene)
		node = scene->mRootNode;
	else
	{
		LOG("The Object does not have an scene. It won't be load");
		ret = false;
	}

	//Create gameObject that contains FBX parts

	GameObject* fbx_obj = App->viewport->CreateGameObject(path_fbx);
	if (scene && scene->HasMeshes())
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			LoadNode(scene->mRootNode->mChildren[i], scene, path_texture, fbx_obj);
		}
	}
	else
	{

		LOG("Error loading FBX with path: %s", path_fbx);
	}

	aiReleaseImport(scene);


	//App->camera->FocusObjectImport(*(fbx_obj->children.begin()));


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
	GameObject* aux_obj = App->viewport->CreateGameObject(node->mName.C_Str(), parent, pos, scale, rot);

	if (node->mNumMeshes > 0)
	{
		//Load Mesh 
		ComponentMesh* m = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
		const aiMesh* aimesh = scene->mMeshes[node->mMeshes[0]];

		m->transform = aux_obj->GetComponentTransform();

		for (uint i = 0; i < aimesh->mNumVertices; i++)
		{
			m->vertices.push_back(float3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z));
		}

		LOG("New mesh with %d vertices", m->vertices.size());

		if (aimesh->HasFaces())
		{
			for (uint i = 0; i < aimesh->mNumFaces; i++) //ASSUME FACE IS TRIANGLE
			{
				aiFace aiface = aimesh->mFaces[i];
				for (uint j = 0; j < aiface.mNumIndices; j++)
				{
					m->index.push_back(aiface.mIndices[j]);
				}
			}
			
		}

		//Load Normals
		if (aimesh->HasNormals())
		{
			m->normals = new float3[aimesh->mNumVertices];
			memcpy(m->normals, aimesh->mNormals, sizeof(aiVector3D) * m->vertices.size());

			for (uint i = 0; i < m->index.size(); i += 3)
			{
				uint index = m->index[i];
				vec3 vertex0(m->vertices.at(index).x, m->vertices.at(index).y, m->vertices.at(index).z);

				index = m->index[i + 1];
				vec3 vertex1(m->vertices.at(index).x, m->vertices.at(index).y, m->vertices.at(index).z);

				index = m->index[i + 2];
				vec3 vertex2(m->vertices.at(index).x, m->vertices.at(index).y, m->vertices.at(index).z);
				CalculateNormalTriangle(m, vertex0, vertex1, vertex2);
			}
		}

		//Load UVs
		if (aimesh->HasTextureCoords(0))
		{
			m->UV_num = aimesh->mNumUVComponents[0];
			m->UV_coord = new float[m->vertices.size() * m->UV_num];

			for (uint i = 0; i < m->vertices.size(); i++)
			{
				memcpy(&m->UV_coord[i * m->UV_num], &aimesh->mTextureCoords[0][i], sizeof(float) * m->UV_num);
			}

		}

		m->GenerateMesh();
		m->local_AABB.SetNegativeInfinity();
		m->local_AABB.Enclose(m->vertices.data(), m->vertices.size());

		//Generate Texture
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

	if (App->viewport->selected_object) {
		ComponentTexture* texture = App->viewport->selected_object->GetComponentTexture();
		ComponentMesh* mesh = App->viewport->selected_object->GetComponentMesh();

		if (!texture)
			texture = (ComponentTexture*)App->viewport->selected_object->CreateComponent(COMPONENT_TEXTURE);

		texture->LoadTexture(path_texture);
		if (mesh)
			mesh->image_id = texture->image_id;
		else
		{
			LOG("The object does not have a MESH! Create one or select another object.");
			App->viewport->selected_object->DeleteComponent(texture);
		}
	}
	else
	{
		C_WARNING("Warning! Object no selected. Please, select an object.");
	}
	return true;
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
	trans->local_position.x = 0;
	trans->local_position.y = 0;
	trans->local_position.z = 0;

	par_shapes_scale(shape, 1.0f, 1.0f, 1.0f);
	trans->local_scale.x = 1;
	trans->local_scale.y = 1;
	trans->local_scale.z = 1;

	trans->local_rotation = Quat::identity;
	trans->local_rotation_euler.x = 0;
	trans->local_rotation_euler.y = 0;
	trans->local_rotation_euler.z = 0;

	//Create Component Mesh
	ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(COMPONENT_MESH);
	for (uint i = 0; i < shape->npoints * 3;)
	{
		mesh->vertices.push_back(float3(shape->points[i], shape->points[i + 1], shape->points[i + 2]));
		i += 3;
	}

	mesh->index.insert(mesh->index.end(), &shape->triangles[0], &shape->triangles[shape->ntriangles * 3]);
	
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