#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "Scene.h"
#include "ComponentMesh.h"

#include "MathGeoLib/include/Math/Quat.h"

//-------------- Assimp --------------
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

ResourceModel::ResourceModel():Resource()
{
	type = resource_type::RESOURCE_MODEL;
}

bool ResourceModel::ImportModel(const char * path, std::string& output_file)
{
	bool ret = true;

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		ResourceModel model;

		std::vector<Random> textures, meshes;

		model.GenerateTexture(scene, path, textures);
		model.GenerateMeshes(scene, path, meshes);
		model.GenerateNodes(scene, scene->mRootNode, 0, meshes, textures);
		aiReleaseImport(scene);

		ret = Save(model, output_file);
	}
	else
	{
		LOG("The Object does not have an scene. It won't be load");
		ret = false;
	}

	aiDetachAllLogStreams();
	
	return ret;
}

bool ResourceModel::Load()
{
	Scene* model = new Scene(imported_file.c_str());
	Scene model_array = model->GetArray("Model");

	for (int i = 0;;i++)
	{
		if (model->IsArraySection(i))
		{
			Scene go = model_array.GetSectionArray(i);
			GameObject* parent = App->viewport->CreateGameObject(go.GetString("Name").c_str(), nullptr, go.GetFloat3("Position"), go.GetFloat3("Scale"), go.GetQuat("Rotation"));

			ResourceMesh* mesh = new ResourceMesh();
			mesh = mesh->Load(go);
			if (mesh)
			{
				ComponentMesh* comp_mesh = (ComponentMesh*)parent->CreateComponent(COMPONENT_MESH);
				comp_mesh->AddMesh(mesh);
			}
			
		}
		else
			break;
	}
	

	return true;
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

	ResourceTexture texture;

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		materials.push_back(texture.Import(scene->mMaterials[i], file));
	}
}

void ResourceModel::GenerateMeshes(const aiScene* scene, const char* file, std::vector<Random>& meshes)
{
	meshes.reserve(scene->mNumMeshes);

	ResourceMesh mesh;

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		meshes.push_back(mesh.Import(scene->mMeshes[i], file));
	}
}

void ResourceModel::GenerateNodes(const aiScene * model, const aiNode * node, uint parent, const std::vector<Random>& meshes, const std::vector<Random>& textures)
{
	uint index = nodes.size();
	Node dst;

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);

	dst.position = float3(translation.x, translation.y, translation.z);
	dst.rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
	dst.scale = float3(scaling.x, scaling.y, scaling.z);
	dst.name = node->mName.C_Str();
	dst.parent = parent;

	nodes.push_back(dst);

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		GenerateNodes(model, node->mChildren[i], index, meshes, textures);
	}


	if (node->mNumMeshes > 0)
	{
		uint mesh_index = node->mMeshes[0];

		nodes[index].mesh = meshes[mesh_index];
		nodes[index].texture = textures[model->mMeshes[mesh_index]->mMaterialIndex];
	}
}

bool ResourceModel::Save(ResourceModel model, std::string & output) const
{
	bool ret = false;
	Scene* model_scene = new Scene();

	Scene models = model_scene->AddArray("Model");
	Scene actual_node;

	//Save
	for (uint i = 0; i < model.nodes.size(); ++i)
	{
		actual_node = models.AddSectionArray(i);
		actual_node.AddString("Name", model.nodes[i].name);
		actual_node.AddFloat3("Position", model.nodes[i].position);
		actual_node.AddQuat("Rotation", model.nodes[i].rotation);
		actual_node.AddFloat3("Scale", model.nodes[i].scale);
		actual_node.AddDouble("Parent", model.nodes[i].parent);
		actual_node.AddDouble("Mesh", model.nodes[i].mesh.GetNumber());
		actual_node.AddDouble("Texture", model.nodes[i].texture.GetNumber());
	}

	output = model.nodes[0].name + ".model";
	ret = model_scene->Save(output.c_str());
	//ret = App->fs->CopyFromOutsideFS(output.c_str(), std::string(LIBRARY_MODELS_FOLDER + output).c_str());
	//App->fs->Remove(output.c_str());
	return ret;
}
