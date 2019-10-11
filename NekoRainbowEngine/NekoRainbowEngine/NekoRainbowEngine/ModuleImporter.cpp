#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleImporter.h"

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

	FBX* aux_fbx = new FBX();
	const aiScene* scene = aiImportFile(path_fbx, aiProcessPreset_TargetRealtime_MaxQuality);

	ret = LoadMesh(scene, aux_fbx, path_fbx, path_texture);

	return ret;
}

bool ModuleImporter::LoadMesh(const aiScene* scene, FBX*& aux_fbx, const char*& path_fbx,const char* path_tex)
{
	bool ret = true;

	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			//Load Mesh 
			Mesh* m = new Mesh();
			aiMesh* aimesh = scene->mMeshes[i];

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
				m->UV_coord = new float3[m->num_vertices];
				for (uint i = 0; i < m->num_vertices; i++)
				{
					memcpy(m->UV_coord, &aimesh->mTextureCoords[0][i], sizeof(float3) * m->num_vertices);
				}
				
			}

			m->GenerateMesh();

			if (aimesh->HasTextureCoords(0))
				ret = aux_fbx->LoadTextures(m, path_tex);

			aux_fbx->mesh_list.push_back(m);

		}

		fbx_list.push_back(aux_fbx);
		LOG("Loaded mesh file succesfully!");

	}
	else {
		ret = false;
		LOG("The file with path: %s can not be load", path_fbx);
	}

	return ret;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	for (auto it_fbx = fbx_list.begin(); it_fbx != fbx_list.end(); ++it_fbx)
	{
		//Render Mesh
		for (auto it_mesh = (*it_fbx)->mesh_list.begin(); it_mesh != (*it_fbx)->mesh_list.end(); ++it_mesh)
		{
			(*it_mesh)->Render();
		}

		//Render Texture
		if((*it_fbx)->texture)
			(*it_fbx)->texture->Render();
	}

	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	for (auto it_fbx = fbx_list.begin(); it_fbx != fbx_list.end(); ++it_fbx)
	{
		delete (*it_fbx);
		(*it_fbx) = nullptr;
	}

	aiDetachAllLogStreams();
	return true;
}