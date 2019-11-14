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
#include "MathGeoLib/include/Math/float2.h"
#include "Mesh.h"

bool MeshImporter::Init()
{

	return true;
}

bool MeshImporter::CleanUp()
{
	return true;
}

Mesh* MeshImporter::Import(const aiScene * scene, const aiMesh * aimesh)
{
	Mesh* mesh = new Mesh();

	//Vertices Load
	mesh->vertices_size = aimesh->mNumVertices;
	mesh->vertices = new float3[aimesh->mNumVertices];
	memcpy(mesh->vertices, aimesh->mVertices, sizeof(float3) * aimesh->mNumVertices);
	LOG("New mesh with %d vertices", mesh->vertices_size);

	//Index Load
	if (aimesh->HasFaces())
	{
		mesh->index_size = aimesh->mNumFaces * 3;
		mesh->index = new uint[mesh->index_size];
		for (uint i = 0; i < aimesh->mNumFaces; i++) //ASSUME FACE IS TRIANGLE
		{
			memcpy(&mesh->index[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	//Load Normals
	if (aimesh->HasNormals())
	{
		CalculateNormalFace(mesh, aimesh);
	}

	//Load UVs
	if (aimesh->HasTextureCoords(0))
	{
		mesh->UV_size = mesh->vertices_size;
		mesh->UV_coord = new float2[mesh->UV_size];

		for (uint i = 0; i < mesh->UV_size; i++)
		{
			float2 uv = float2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y);
			mesh->UV_coord[i] = uv;
		}

	}

	mesh->GenerateBuffers();

	LOG("Loaded mesh file succesfully!");

	return mesh;
}

void MeshImporter::CalculateNormalFace(Mesh * mesh, const aiMesh * aimesh)
{
	/*mesh->normals_vertex = new float3[aimesh->mNumVertices];
	memcpy(mesh->normals_vertex, aimesh->mNormals, sizeof(aiVector3D) * mesh->vertices_size);

	std::vector<float3> normal_face;
	for (uint i = 0; i < mesh->vertices_size; i += 3)
	{
		uint index = mesh->index[i];
		vec3 vertex0(mesh->vertices[index].x, mesh->vertices[index].y, mesh->vertices[index].z);

		index = mesh->index[i + 1];
		vec3 vertex1(mesh->vertices[index].x, mesh->vertices[index].y, mesh->vertices[index].z);

		index = mesh->index[i + 2];
		vec3 vertex2(mesh->vertices[index].x, mesh->vertices[index].y, mesh->vertices[index].z);

		vec3 center = (vertex0 + vertex1 + vertex2) / 3;

		vec3 vec_v = vertex0 - vertex2;
		vec3 vec_w = vertex1 - vertex2;

		vec3 norm_v = cross(vec_v, vec_w);
		norm_v = normalize(norm_v);

		normal_face.push_back(float3(center.x, center.y, center.z));
		normal_face.push_back(float3(norm_v.x, norm_v.y, norm_v.z));

		mesh->norm_face_size += 2;
	}

	mesh->normals_vertex = new float3[mesh->norm_face_size];
	std::copy(normal_face.begin(), normal_face.end(), mesh->normals_vertex);*/
}

bool MeshImporter::SaveMesh(Mesh * mesh, const char* name)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[3] = { mesh->index_size, mesh->vertices_size, mesh->UV_size};
	uint size = sizeof(ranges) + sizeof(uint) * mesh->index_size + sizeof(float3) * mesh->vertices_size + sizeof(float2)*mesh->UV_size;

	// Allocate
	char* data = new char[size];
	char* cursor = data;

	// First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh->index_size;
	memcpy(cursor, mesh->index, bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float3) * mesh->vertices_size;
	memcpy(cursor, mesh->vertices, bytes);
	cursor += bytes;

	if (mesh->UV_size > 0) 
	{
		// Store UV
		bytes = sizeof(float2) * mesh->UV_size;
		memcpy(cursor, mesh->UV_coord, bytes);
		cursor += bytes;
	}

	std::string mesh_name = LIBRARY_MESH_FOLDER;
	mesh_name += name;
	uint ret = App->fs->Save(std::string(mesh_name + ".neko").c_str(), data, size);

	RELEASE_ARRAY(data);
	return true;
}

Mesh* MeshImporter::Load(const char * exported_file)
{
	Mesh* mesh = new Mesh();

	std::string extension, file;
	App->fs->SplitFilePath(exported_file, nullptr, &file, &extension);
	mesh->name = file.c_str();

	std::string path = LIBRARY_MESH_FOLDER;
	path.append(std::string(file + "." + extension).c_str());

	char* buffer;
	uint size = App->fs->Load(path.c_str(), &buffer);

	char* cursor = buffer;
	// Amount of Index/Vertices/UVs
	uint ranges[3];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	mesh->index_size = ranges[0];
	mesh->vertices_size = ranges[1];
	mesh->UV_size = ranges[2];
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

	//Load UV
	if (mesh->UV_size > 0)
	{
		bytes = sizeof(float2) * mesh->UV_size;
		mesh->UV_coord = new float2[mesh->UV_size];
		memcpy(mesh->UV_coord, cursor, bytes);
		cursor += bytes;
	}

	mesh->GenerateBuffers();

	RELEASE_ARRAY(buffer);
	return mesh;
}