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
	for (uint i = 0; i < aimesh->mNumVertices; i++)
	{
		aux_mesh->vertices.push_back(float3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z));
	}
	LOG("New mesh with %d vertices", aux_mesh->vertices.size());

	//Index Load
	if (aimesh->HasFaces())
	{
		for (uint i = 0; i < aimesh->mNumFaces; i++) //ASSUME FACE IS TRIANGLE
		{
			aiFace aiface = aimesh->mFaces[i];
			for (uint j = 0; j < aiface.mNumIndices; j++)
			{
				aux_mesh->index.push_back(aiface.mIndices[j]);
			}
		}
	}

	//Load Normals
	if (aimesh->HasNormals())
	{
		aux_mesh->normals = new float3[aimesh->mNumVertices];
		memcpy(aux_mesh->normals, aimesh->mNormals, sizeof(aiVector3D) * aux_mesh->vertices.size());

		for (uint i = 0; i < aux_mesh->index.size(); i += 3)
		{
			uint index = aux_mesh->index[i];
			vec3 vertex0(aux_mesh->vertices.at(index).x, aux_mesh->vertices.at(index).y, aux_mesh->vertices.at(index).z);

			index = aux_mesh->index[i + 1];
			vec3 vertex1(aux_mesh->vertices.at(index).x, aux_mesh->vertices.at(index).y, aux_mesh->vertices.at(index).z);

			index = aux_mesh->index[i + 2];
			vec3 vertex2(aux_mesh->vertices.at(index).x, aux_mesh->vertices.at(index).y, aux_mesh->vertices.at(index).z);
			CalculateNormalTriangle(aux_mesh, vertex0, vertex1, vertex2);
		}
	}

	//Load UVs
	if (aimesh->HasTextureCoords(0))
	{
		aux_mesh->UV_num = aimesh->mNumUVComponents[0];
		aux_mesh->UV_coord = new float[aux_mesh->vertices.size() * aux_mesh->UV_num];

		for (uint i = 0; i < aux_mesh->vertices.size(); i++)
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
	uint ranges[2] = { mesh->index.size(), mesh->vertices.size()};
	uint size = sizeof(ranges) + sizeof(uint) * mesh->index.size() + sizeof(float3) * mesh->vertices.size();

	// Allocate
	char* data = new char[size];
	char* cursor = data;

	// First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh->index.size();
	memcpy(cursor, &mesh->index[0], bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float3) * mesh->vertices.size();
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
	mesh->index.resize(0);
	mesh->vertices.resize(0);
	cursor += bytes;

	// Load indices
	bytes = sizeof(uint) * mesh->index.size();
	uint* index = new uint[mesh->index.size()];
	memcpy(index, cursor, bytes);
	for(int i = 0; i < ranges[0]; i++)
	{
		mesh->index.push_back(index[i]);
	}
	cursor += bytes;

	// Load vertices
	bytes = sizeof(float3) * mesh->vertices.size();
	float3* vertex = new float3[mesh->vertices.size()];
	memcpy(vertex, cursor, bytes);
	for (int i = 0; i < ranges[1]; i++)
	{
		mesh->vertices.push_back(vertex[i]);
	}
	cursor += bytes;

	mesh->transform = mesh->my_go->GetComponentTransform();
	mesh->CreateLocalAABB();
	mesh->GetGlobalAABB();
	mesh->GenerateBuffers();

	RELEASE_ARRAY(buffer);

	return mesh;
}