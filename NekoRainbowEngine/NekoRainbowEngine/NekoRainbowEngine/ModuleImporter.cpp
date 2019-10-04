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

void ModuleImporter::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, fbx_1.mesh_list.begin()->num_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx_1.mesh_list.begin()->num_index);
	glDrawElements(GL_TRIANGLES, fbx_1.mesh_list.begin()->num_index * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImporter::LoadFile(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		uint i = 0;
		for (aiMesh* (*it) = scene->mMeshes; i <= scene->mNumMeshes; i++)
		{
			Mesh m;
			m.num_vertices = (*it)->mNumVertices;
			m.vertices = new float[m.num_vertices * 3];
			memcpy(m.vertices, (*it)->mVertices, sizeof(float) * m.num_vertices * 3);
			LOG("New mesh with %d vertices", m.num_vertices);

			if ((*it)->HasFaces())
			{
				m.num_index = (*it)->mNumFaces * 3;
				m.index = new uint[m.num_index]; // assume each face is a triangle
				for (uint i = 0; i < (*it)->mNumFaces; ++i)
				{
					if ((*it)->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m.index[i * 3], (*it)->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}
			fbx_1.mesh_list.push_back(m);
		}
	}
}

void ModuleImporter::GenerateMesh()
{
	std::list<Mesh>::iterator mesh = fbx_1.mesh_list.begin();
	//Cube Vertex
	mesh->id_vertex = 0;
	glGenBuffers(1, (GLuint*) &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mesh->num_vertices* 3, mesh->vertices, GL_STATIC_DRAW);

	//Cube Vertex definition
	mesh->id_index = 0;
	glGenBuffers(1, (GLuint*) &(mesh->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_index * 3, mesh->index, GL_STATIC_DRAW);
}
