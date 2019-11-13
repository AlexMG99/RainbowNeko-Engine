#include "Application.h"
#include "ModuleImporter.h"
#include "SceneImporter.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "Mesh.h"

//-------------- Assimp --------------
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"

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
		LOG("Error loading FBX with path: %s", path);

	aiReleaseImport(scene);
	App->camera->FocusObject(*(fbx_obj->children.begin()));
	return ret;
}

void SceneImporter::LoadNode(const aiNode * node, const aiScene * scene, const char * path_fbx, GameObject* parent)
{
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
		ComponentMesh* comp_mesh = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
		ComponentTexture* tex = nullptr;
		const aiMesh* aimesh = scene->mMeshes[node->mMeshes[0]];
		Mesh* mesh = App->importer->mesh_imp->Import(scene, aimesh); 

		//Load Material
		if (aimesh->mMaterialIndex >= 0)
		{
			aiString texture_path;
			scene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
			if (texture_path.length > 0)
			{
				std::string output_file, file, extension;
				App->fs->SplitFilePath(texture_path.C_Str(), nullptr, &file, &extension);

				App->importer->texture_imp->ImportTexture(file.c_str(), output_file);
				tex = App->importer->texture_imp->Load(std::string("." + output_file).c_str());
				comp_mesh->image_id = tex->image_id;
				aux_obj->AddComponent(tex);
			}
		}
		App->importer->mesh_imp->SaveMesh(mesh);

		comp_mesh->transform = aux_obj->GetComponentTransform();
		comp_mesh->AddMesh(mesh);
		comp_mesh->CreateLocalAABB();
		comp_mesh->GetGlobalAABB();
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, path_fbx, aux_obj);
	}
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