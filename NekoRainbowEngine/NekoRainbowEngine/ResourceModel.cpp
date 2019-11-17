#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"

//-------------- Assimp --------------
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

#include "MathGeoLib/include/Math/Quat.h"

//bool SceneImporter::Init()
//{
//	struct aiLogStream stream;
//	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
//	stream.callback = LogCallback;
//	aiAttachLogStream(&stream);
//
//	return true;
//}
//
//bool SceneImporter::CleanUp()
//{
//	aiDetachAllLogStreams();
//	return true;
//}

bool ResourceModel::ImportModel(const char * path, std::string output_file)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		ResourceModel model;

		std::vector<Random> materials, meshes;

		model.GenerateTexture(scene, path, materials);
		model.GenerateMeshes(scene, path, meshes);
	}
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

	fbx_obj->CreateTransformAABB();

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
	GameObject* aux_obj;

	if (node->mNumMeshes > 0)
	{
		aux_obj = App->viewport->CreateGameObject(node->mName.C_Str(), parent, pos, scale, rot);
		ComponentMesh* comp_mesh = (ComponentMesh*)aux_obj->CreateComponent(COMPONENT_MESH);
		ComponentTexture* comp_text = (ComponentTexture*)aux_obj->CreateComponent(COMPONENT_TEXTURE);
		const aiMesh* aimesh = scene->mMeshes[node->mMeshes[0]];
		ResourceMesh* mesh = 
		mesh->name = aux_obj->GetName();


		App->importer->mesh_imp->SaveMesh(mesh, mesh->name.c_str());

		comp_mesh->transform = aux_obj->GetComponentTransform();
		comp_mesh->AddMesh(mesh);
	}
	else
		aux_obj = parent;

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

void ResourceModel::GenerateTexture(const aiScene* scene, const char* file, std::vector<Random>& materials)
{
	materials.reserve(scene->mNumMaterials);

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		materials.push_back(ResourceTexture::Import(scene->mMaterials[i], file));
	}
}

void ResourceModel::GenerateMeshes(const aiScene* scene, const char* file, std::vector<Random>& meshes)
{
	meshes.reserve(scene->mNumMeshes);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		meshes.push_back(ResourceMesh::Import(scene->mMeshes[i], file));
	}
}