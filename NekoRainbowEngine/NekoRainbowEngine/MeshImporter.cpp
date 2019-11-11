#include "Globals.h"
#include "ComponentMesh.h"
#include "MeshImporter.h"

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
	return true;
}

ComponentMesh * MeshImporter::Load(const char * exported_file)
{
	return nullptr;
}