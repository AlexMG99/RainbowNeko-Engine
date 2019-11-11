#include "Application.h"
#include "SceneImporter.h"
#include "GameObject.h"
#include "ComponentMesh.h"

//-------------- Assimp --------------
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

#include "MathGeoLib/include/Math/Quat.h"

bool SceneImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool SceneImporter::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

bool SceneImporter::Import(const char * path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	const aiNode* node;

	if (scene)
		node = scene->mRootNode;
	else
	{
		LOG("The Object does not have an scene. It won't be load");
		ret = false;
	}

	//Create gameObject that contains FBX parts

	GameObject* fbx_obj = App->viewport->CreateGameObject(path);
	if (scene && scene->HasMeshes())
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			LoadNode(scene->mRootNode->mChildren[i], scene, path, fbx_obj);
		}
	}
	else
	{
		LOG("Error loading FBX with path: %s", path);
	}

	aiReleaseImport(scene);

	App->camera->FocusObject(*(fbx_obj->children.begin()));

	return ret;
}

void SceneImporter::LoadNode(const aiNode * node, const aiScene * scene, const char * path_fbx, GameObject* parent)
{
	//BROFILER_CATEGORY("LoadNode_ModuleImporter", Profiler::Color::LightGoldenRodYellow);

	//Get Component transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	//Create aux_obj
	GameObject* aux_obj = App->viewport->CreateGameObject(node->mName.C_Str(), parent, pos, scale, rot);

	if (node->mNumMeshes > 0)
	{
		//Load Mesh 
		ComponentMesh* m = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
		const aiMesh* aimesh = scene->mMeshes[node->mMeshes[0]];

		m->transform = aux_obj->GetComponentTransform();

		for (uint i = 0; i < aimesh->mNumVertices; i++)
		{
			m->vertices.push_back(float3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z));
		}

		LOG("New mesh with %d vertices", m->vertices.size());

		if (aimesh->HasFaces())
		{
			for (uint i = 0; i < aimesh->mNumFaces; i++) //ASSUME FACE IS TRIANGLE
			{
				aiFace aiface = aimesh->mFaces[i];
				for (uint j = 0; j < aiface.mNumIndices; j++)
				{
					m->index.push_back(aiface.mIndices[j]);
				}
			}
		}

		//Load Material
		if (aimesh->mMaterialIndex >= 0)
		{
			aiString texture_path;
			scene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
			if (texture_path.length > 0)
			{
				std::string file, extension;
				App->fs->SplitFilePath(texture_path.C_Str(), nullptr, &file, &extension);

				//Import(path_fbx, path_texture, );
				ComponentTexture* texture = (ComponentTexture*)aux_obj->CreateComponent(COMPONENT_TEXTURE);
				texture->LoadTexture(file.c_str());
				m->image_id = texture->image_id;
			}
		}
		//Load Normals
		if (aimesh->HasNormals())
		{
			m->normals = new float3[aimesh->mNumVertices];
			memcpy(m->normals, aimesh->mNormals, sizeof(aiVector3D) * m->vertices.size());

			for (uint i = 0; i < m->index.size(); i += 3)
			{
				uint index = m->index[i];
				vec3 vertex0(m->vertices.at(index).x, m->vertices.at(index).y, m->vertices.at(index).z);

				index = m->index[i + 1];
				vec3 vertex1(m->vertices.at(index).x, m->vertices.at(index).y, m->vertices.at(index).z);

				index = m->index[i + 2];
				vec3 vertex2(m->vertices.at(index).x, m->vertices.at(index).y, m->vertices.at(index).z);
				CalculateNormalTriangle(m, vertex0, vertex1, vertex2);
			}
		}

		//Load UVs
		if (aimesh->HasTextureCoords(0))
		{
			m->UV_num = aimesh->mNumUVComponents[0];
			m->UV_coord = new float[m->vertices.size() * m->UV_num];

			for (uint i = 0; i < m->vertices.size(); i++)
			{
				memcpy(&m->UV_coord[i * m->UV_num], &aimesh->mTextureCoords[0][i], sizeof(float) * m->UV_num);
			}

		}
		m->CreateLocalAABB();
		m->GetGlobalAABB();
		m->GenerateBuffers();

		LOG("Loaded mesh file succesfully!");
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, path_fbx, aux_obj);
	}
}

void SceneImporter::CalculateNormalTriangle(ComponentMesh * m, vec3 triangle_p1, vec3 triangle_p2, vec3 triangle_p3)
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

void LogCallback(const char* text, char* data)
{
	std::string temp_string = text;
	std::size_t pos = temp_string.find(",");
	std::string temp_string2 = temp_string.substr(0, 4);
	temp_string.erase(std::remove(temp_string.begin(), temp_string.end(), '%'), temp_string.end());
	if (temp_string2 == "Info")
	{
		temp_string2 = temp_string.substr(pos + 1);
		LOG(temp_string2.c_str());
	}
	else if (temp_string2 == "Warn")
	{
		temp_string2 = temp_string.substr(pos + 1);
		LOG(temp_string2.c_str());
	}
}