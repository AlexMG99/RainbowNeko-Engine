#include "Globals.h"
#include "Application.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "MeshImporter.h"

#include <string>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"

bool MeshImporter::Init()
{

	return true;
}

bool MeshImporter::CleanUp()
{
	return true;
}

ComponentMesh* MeshImporter::Import(const aiScene * scene, const aiMesh * aimesh)
{
	ComponentMesh* aux_mesh = new ComponentMesh(COMPONENT_MESH, true, nullptr);

	//Vertices Load
	aux_mesh->vertices_size = aimesh->mNumVertices;
	aux_mesh->vertices = new float3[aimesh->mNumVertices];
	memcpy(aux_mesh->vertices, aimesh->mVertices, sizeof(float3) * aimesh->mNumVertices);
	LOG("New mesh with %d vertices", aux_mesh->vertices_size);

	//Index Load
	if (aimesh->HasFaces())
	{
		aux_mesh->index_size = aimesh->mNumFaces * 3;
		aux_mesh->index = new uint[aux_mesh->index_size];
		for (uint i = 0; i < aimesh->mNumFaces; i++) //ASSUME FACE IS TRIANGLE
		{
			memcpy(&aux_mesh->index[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	//Load Normals
	if (aimesh->HasNormals())
	{
		aux_mesh->normals = new float3[aimesh->mNumVertices];
		memcpy(aux_mesh->normals, aimesh->mNormals, sizeof(aiVector3D) * aux_mesh->vertices_size);

		for (uint i = 0; i < aux_mesh->vertices_size; i += 3)
		{
			uint index = aux_mesh->index[i];
			vec3 vertex0(aux_mesh->vertices[index].x, aux_mesh->vertices[index].y, aux_mesh->vertices[index].z);

			index = aux_mesh->index[i + 1];
			vec3 vertex1(aux_mesh->vertices[index].x, aux_mesh->vertices[index].y, aux_mesh->vertices[index].z);

			index = aux_mesh->index[i + 2];
			vec3 vertex2(aux_mesh->vertices[index].x, aux_mesh->vertices[index].y, aux_mesh->vertices[index].z);
			CalculateNormalTriangle(aux_mesh, vertex0, vertex1, vertex2);
		}
	}

	//Load UVs
	if (aimesh->HasTextureCoords(0))
	{
		aux_mesh->UV_num = aimesh->mNumUVComponents[0];
		aux_mesh->UV_coord = new float[aux_mesh->vertices_size * aux_mesh->UV_num];

		for (uint i = 0; i < aux_mesh->vertices_size; i++)
		{
			memcpy(&aux_mesh->UV_coord[i * aux_mesh->UV_num], &aimesh->mTextureCoords[0][i], sizeof(float) * aux_mesh->UV_num);
		}

	}
	LOG("Loaded mesh file succesfully!");

	return aux_mesh;
}

void MeshImporter::CalculateNormalTriangle(ComponentMesh * m, vec3 triangle_p1, vec3 triangle_p2, vec3 triangle_p3)
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

bool MeshImporter::SaveMesh(ComponentMesh * mesh)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[2] = { mesh->index_size, mesh->vertices_size };
	uint size = sizeof(ranges) + sizeof(uint) * mesh->index_size + sizeof(float3) * mesh->vertices_size;

	// Allocate
	char* data = new char[size];
	char* cursor = data;

	// First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh->index_size;
	memcpy(cursor, &mesh->index[0], bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float3) * mesh->vertices_size;
	memcpy(cursor, &mesh->vertices[0], bytes);
	cursor += bytes;

	std::string mesh_name = LIBRARY_MESH_FOLDER;
	mesh_name += mesh->my_go->GetName().c_str();
	uint ret = App->fs->Save(std::string(mesh_name + ".neko").c_str(), data, size);
	RELEASE_ARRAY(data);

	return true;
}

ComponentMesh * MeshImporter::Load(const char * exported_file)
{
	GameObject* obj = App->viewport->CreateGameObject(exported_file);
	ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(COMPONENT_MESH);

	std::string normalized_path = exported_file;
	App->fs->NormalizePath(normalized_path);

	std::string extension, file;
	App->fs->SplitFilePath(normalized_path.c_str(), nullptr, &file, &extension);

	std::string path = LIBRARY_MESH_FOLDER;
	path.append(file.c_str());

	char* buffer;
	uint size = App->fs->Load(path.c_str(), &buffer);

	char* cursor = buffer;
	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	mesh->index_size = ranges[0];
	mesh->vertices_size = ranges[1];
	cursor += bytes;

	// Load indices
	bytes = sizeof(uint) * mesh->index_size;
	mesh->index = new uint[mesh->index_size];
	memcpy(mesh->index, cursor, bytes);
	cursor += bytes;

	// Load vertices
	bytes = sizeof(float3) * mesh->vertices_size;
	mesh->vertices = new float3[mesh->vertices_size];
	memcpy(mesh->vertices, cursor, bytes);
	cursor += bytes;

	mesh->transform = mesh->my_go->GetComponentTransform();
	mesh->CreateLocalAABB();
	mesh->GetGlobalAABB();
	mesh->GenerateBuffers();

	RELEASE_ARRAY(buffer);

	return mesh;
}