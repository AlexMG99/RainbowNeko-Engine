#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "PanelConsole.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"


#pragma comment(lib, "Assimp/libx86/assimp.lib")

#include <list>

//----------------- ModuleImporter -----------------//

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module (app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImporter::Start()
{
	bool ret = true;
	//ret = ImportFBX("../Game/Assets/BakerHouse.fbx", "../Game/Assets/Baker_house.dds");

	return ret;
}

bool ModuleImporter::ImportFBX(char* path_fbx, char* path_texture)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path_fbx, aiProcessPreset_TargetRealtime_MaxQuality);
	const aiNode* node = scene->mRootNode;

	RecursiveChild(node, path_fbx, scene, path_texture);

	return ret;
}

void ModuleImporter::RecursiveChild(const aiNode * node, char * path_fbx, const aiScene * scene, char * path_texture)
{
	for (uint node_num = 0; node_num < node->mNumChildren; node_num++)
	{
		RecursiveChild(node->mChildren[node_num], path_fbx, scene, path_texture);
	}
	CreateObject(node, path_fbx, scene, path_texture);
}

void ModuleImporter::CreateObject(const aiNode * node, char * path_fbx, const aiScene * scene, char * path_texture)
{
	if (node->mNumMeshes > 0)
	{
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			//Load Position
			GameObject* aux_obj = new GameObject();
			std::string name_obj = path_fbx + std::to_string(App->viewport->root_object->children.size());
			aux_obj->SetName(name_obj.c_str());

			aiVector3D translation, scaling;
			aiQuaternion rotation;
			node->mTransformation.Decompose(scaling, rotation, translation);
			float3 pos(translation.x, translation.y, translation.z);
			float3 scale(scaling.x, scaling.y, scaling.z);
			Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			ComponentTransform* trans = (ComponentTransform*)aux_obj->CreateComponent(COMPONENT_TRANSFORM);
			trans->position[0] = pos.x;
			trans->position[1] = pos.y;
			trans->position[2] = pos.z;
			trans->scale[0] = scale.x;
			trans->scale[1] = scale.y;
			trans->scale[2] = scale.z;
			trans->rotation = rot;

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
			App->viewport->CreateGameObject(aux_obj, true);
		}

	}
}

bool ModuleImporter::ImportTexture(char * path_texture)
{
	GameObject* object = nullptr;

	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		if ((*it_obj)->selected)
			object = (*it_obj);

	}

	if (object) {
		ComponentTexture* texture = (ComponentTexture*)object->CreateComponent(COMPONENT_TEXTURE);
		texture->LoadTexture(path_texture);
		object->GetComponentMesh()->image_id = texture->image_id;
	}
	else
	{
		C_WARNING("Warning! Object no selected. Please, select an object.");
	}
	return false;
}

update_status ModuleImporter::PostUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
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