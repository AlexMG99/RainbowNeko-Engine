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
	App->importer->LoadFile("../Game/Assets/warrior.fbx");

	for (std::list<FBX*>::iterator it_fbx = fbx_list.begin(); it_fbx != fbx_list.end(); ++it_fbx)
	{
		for (std::list<Mesh*>::iterator it_mesh = (*it_fbx)->mesh_list.begin(); it_mesh != (*it_fbx)->mesh_list.end(); ++it_mesh)
		{
			(*it_mesh)->GenerateMesh();
		}
	}
	return true;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	for (std::list<FBX*>::iterator it_fbx = fbx_list.begin(); it_fbx != fbx_list.end(); ++it_fbx)
	{
		for (std::list<Mesh*>::iterator it_mesh = (*it_fbx)->mesh_list.begin(); it_mesh != (*it_fbx)->mesh_list.end(); ++it_mesh)
		{
			(*it_mesh)->Render();
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImporter::LoadFile(const char* path)
{
	FBX* aux_fbx = new FBX();
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
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
			aux_fbx->mesh_list.push_back(m);
			
		}
		fbx_list.push_back(aux_fbx);
	}
}

void Mesh::GenerateMesh()
{
	//Cube Vertex
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num_vertices* 3, vertices, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_index, index, GL_STATIC_DRAW);

	LOG("Generated mesh with id vertex: %i and id index: %i", id_vertex, id_index);
}

void Mesh::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}
