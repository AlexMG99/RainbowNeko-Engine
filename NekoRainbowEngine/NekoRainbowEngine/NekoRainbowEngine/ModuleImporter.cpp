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
	//App->importer->LoadFile("../Game/Assets/warrior.fbx");

	return true;
}

update_status ModuleImporter::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	for (auto it_fbx = fbx_list.begin(); it_fbx != fbx_list.end(); ++it_fbx)
	{
		for (auto it_mesh = (*it_fbx)->mesh_list.begin(); it_mesh != (*it_fbx)->mesh_list.end(); ++it_mesh)
		{
			(*it_mesh)->Render();
		}
	}

	for (auto it_cube = cube_list.begin(); it_cube != cube_list.end(); ++it_cube)
	{
		(*it_cube)->Render();
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

	for (auto it_cube = cube_list.begin(); it_cube != cube_list.end(); ++it_cube)
	{
		delete (*it_cube);
		(*it_cube) = nullptr;
	}

	aiDetachAllLogStreams();
	return true;
}

bool ModuleImporter::LoadFile(const char* path)
{
	bool ret = true;
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

		for (auto it_fbx = fbx_list.begin(); it_fbx != fbx_list.end(); ++it_fbx)
		{
			if ((*it_fbx) == aux_fbx)
			{
				for (auto it_mesh = (*it_fbx)->mesh_list.begin(); it_mesh != (*it_fbx)->mesh_list.end(); ++it_mesh)
				{

					(*it_mesh)->GenerateMesh();
				}
			}
		}
		LOG("Loaded file succesfully!");
	}
	else {
		ret = false;
		LOG("The file with path: %s can not be load", path);
	}

	return ret;
}

Cube* ModuleImporter::CreateCube(int x, int y, int z)
{
	Cube* cube = new Cube();
	cube->cube_mesh = par_shapes_create_cube();

	par_shapes_translate(cube->cube_mesh, x, y, z);

	cube->GenerateMesh();

	cube_list.push_back(cube);

	return cube;
}

std::list<Cube*> ModuleImporter::GetCubeList() const
{
	return cube_list;
}

Mesh::~Mesh()
{
	delete index;
	index = nullptr;

	delete vertices;
	vertices = nullptr;
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

void Cube::GenerateMesh()
{
	//Cube Vertex
	my_id = 0;
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cube_mesh->npoints * 3, cube_mesh->points, GL_STATIC_DRAW);

	//Cube Vertex definition
	my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*cube_mesh->ntriangles * 3, cube_mesh->triangles, GL_STATIC_DRAW);
}

void Cube::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, cube_mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

FBX::~FBX()
{
	for (auto it_mesh = mesh_list.begin(); it_mesh != mesh_list.end(); ++it_mesh)
	{
		delete (*it_mesh);
		(*it_mesh) = nullptr;
	}
}
