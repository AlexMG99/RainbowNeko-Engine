#include "Application.h"
#include "ModuleResources.h"
#include "ModuleImporter.h"
#include "MeshImporter.h"
#include "Globals.h"
#include "ResourceMesh.h"
#include "Scene.h"
#include "MeshImporter.h"

#include "GL/include/glew.h"
#include "Assimp/include/mesh.h"
#include "MathGeoLib/include/Math/float2.h"

ResourceMesh::ResourceMesh():Resource()
{
	this->type = RESOURCE_MESH;
}

ResourceMesh::ResourceMesh(uint32 id): Resource(id,type)
{
	this->type = RESOURCE_MESH;
}

void ResourceMesh::GenerateBuffers()
{
	//Cube Vertex
	glGenBuffers(1, &buffers[BUFF_VERTICES]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFF_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices_size, vertices, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &buffers[BUFF_INDEX]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[BUFF_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*index_size, index, GL_STATIC_DRAW);

	//UVs definition
	if (UV_size > 0)
	{
		glGenBuffers(1, &buffers[BUFF_UV]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFF_UV]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2)*UV_size, UV_coord, GL_STATIC_DRAW);
	}

	//Normal Definition
	if (norm_face_size > 0)
	{
		glGenBuffers(1, &buffers[BUFF_NORMAL_FACE]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFF_NORMAL_FACE]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * norm_face_size, normals_face, GL_STATIC_DRAW);
	}

	if (norm_vertex_size > 0)
	{
		glGenBuffers(1, &buffers[BUFF_NORMAL_FACE]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFF_NORMAL_FACE]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * norm_vertex_size, normals_vertex, GL_STATIC_DRAW);
	}

	LOG("Generated mesh with id vertex: %i and id index: %i", buffers[BUFF_VERTICES], buffers[BUFF_INDEX]);
}

bool ResourceMesh::Load()
{
	return App->importer->mesh_imp->Load(this);;
}

ResourceMesh* ResourceMesh::Load(Scene& scene)
{
	return App->resources->ImportMesh(scene.GetDouble("Mesh"));
}

ResourceMesh::~ResourceMesh()
{
	RELEASE_ARRAY(UV_coord);
	RELEASE_ARRAY(normals_face);
	RELEASE_ARRAY(normals_vertex);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(index);

	glDeleteBuffers(1, &buffers[BUFF_INDEX]);
	glDeleteBuffers(1, &buffers[BUFF_VERTICES]);
	glDeleteBuffers(1, &buffers[BUFF_NORMAL]);
	glDeleteBuffers(1, &buffers[BUFF_NORMAL_FACE]);
	glDeleteBuffers(1, &buffers[BUFF_UV]);
}

Random ResourceMesh::Import(const aiMesh * mesh, const char * source_file)
{
	ResourceMesh* resource_mesh = (ResourceMesh*)App->resources->CreateNewResource(resource_type::RESOURCE_MESH);
	
	resource_mesh = App->importer->mesh_imp->Import(mesh, resource_mesh);
	App->importer->mesh_imp->SaveMesh(resource_mesh);
	resource_mesh->file = source_file;

	return resource_mesh->GetID();
}

void ResourceMesh::ReleaseFromMemory() 
{
	RELEASE_ARRAY(UV_coord);
	RELEASE_ARRAY(normals_face);
	RELEASE_ARRAY(normals_vertex);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(index);

	glDeleteBuffers(1, &buffers[BUFF_INDEX]);
	glDeleteBuffers(1, &buffers[BUFF_VERTICES]);
	glDeleteBuffers(1, &buffers[BUFF_NORMAL]);
	glDeleteBuffers(1, &buffers[BUFF_NORMAL_FACE]);
	glDeleteBuffers(1, &buffers[BUFF_UV]);
}

