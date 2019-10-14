#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

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
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImporter::Start()
{
	bool ret = true;
	ret = ImportFBX("../Game/Assets/BakerHouse.fbx", "../Game/Assets/Baker_house.dds");

	return ret;
}

bool ModuleImporter::ImportFBX(const char* path_fbx, char* path_texture)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path_fbx, aiProcessPreset_TargetRealtime_MaxQuality);
	const aiNode* node = scene->mRootNode;

	for (uint node_num = 0; node_num < node->mNumChildren; node_num++) 
	{
		if (scene->HasMeshes())
		{
			GameObject* aux_obj = new GameObject();

			for (uint i = 0; i < node->mChildren[node_num]->mNumMeshes; i++)
			{
				//Load Position
				aiVector3D translation, scaling;
				aiQuaternion rotation;
				node->mTransformation.Decompose(scaling, rotation, translation);
				float3 pos(translation.x, translation.y, translation.z);
				float3 scale(scaling.x, scaling.y, scaling.z);
				Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
				ComponentTransform* trans = (ComponentTransform*)aux_obj->CreateComponent(COMPONENT_TRANSFORM);
				trans->position = pos;
				trans->scale = scale;
				trans->rotation = rot;

				//Load Mesh 
				ComponentMesh* m = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
				const aiMesh* aimesh = scene->mMeshes[node->mChildren[node_num]->mMeshes[i]];

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
						//LOG("Vertex: %f, %f, %f", m->UV_coord->x, m->UV_coord->y, m->UV_coord->z);
					}

				}

				m->GenerateMesh();

				/*if (aimesh->HasTextureCoords(0))
					ret = aux_obj->LoadTextures(m, path_tex);*/

			}
			LOG("Loaded mesh file succesfully!");
			App->viewport->CreateGameObject(aux_obj, true);
		}
		else {
			ret = false;
			LOG("The file with path: %s can not be load", path_fbx);
		}
	}
	

	return ret;
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