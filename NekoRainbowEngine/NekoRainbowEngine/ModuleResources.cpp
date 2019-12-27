#include "Application.h"
#include "ModuleResources.h"
#include "ModuleImporter.h"
#include "ModuleFileSystem.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "Random.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "Scene.h"

ModuleResources::ModuleResources(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResources::~ModuleResources()
{

}
bool ModuleResources::CleanUp()
{
	App->fs->Remove(LIBRARY_MESH_FOLDER);
	return true;
}

uint32 ModuleResources::Find(const char * file)
{
	std::string file_name;
	App->fs->SplitFilePath(file, nullptr, &file_name);

	for (std::map<uint32, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->file.compare(file_name) == 0)
			return it->first;
	}

	return 0;
}

Resource* ModuleResources::FindMeta(const char * file)
{
	std::string file_name, extension, path = ".";
	path += ASSETS_META_FOLDER;
	App->fs->SplitFilePath(file, nullptr, &file_name, &extension);
	path += file_name.c_str();
	path += ".meta";

	Scene* meta = new Scene(path.c_str());

	if (meta->GetVRoot())
	{
		Resource* res = Get(meta->GetDouble("ID"));
		if (!res)
		{
			res = CreateNewResource(resource_type(meta->GetInt("Type")), meta->GetDouble("ID"));
		}
		if (res)
		{
			res->file = meta->GetString("File");
			res->imported_file = meta->GetString("ExportedFile");
		}
		if (res->type == RESOURCE_MESH)
		{
			ResourceMesh* mesh = (ResourceMesh*)res;
			mesh->name = meta->GetString("Name");
		}

		if (!res->Load()) 
		{
			ImportMeta(file, path.c_str(), res);
		}

		RELEASE(meta);
		return res;
	}

	RELEASE(meta);

	return nullptr;
}

ResourceMesh* ModuleResources::FindMeshUI(const char * name)
{
	std::string file_name, extension, path = ".";
	path += ASSETS_META_FOLDER;
	App->fs->SplitFilePath(name, nullptr, &file_name, &extension);
	path += file_name.c_str();
	path += ".meta";

	Scene* meta = new Scene(path.c_str());

	if (meta->GetVRoot())
	{
		ResourceMesh* res = (ResourceMesh*)Get(meta->GetDouble("ID"));

		if (!res)
		{
			char id[50];
			uint32 m_id = meta->GetDouble("ID");
			sprintf_s(id, 50, "%u", m_id);
			std::string path = id;
			path += ".neko";
			res = App->importer->mesh_imp->Load(path.c_str());
		}
			

		res->file = meta->GetString("File");
		res->imported_file = meta->GetString("ExportedFile");

		return res;
	}

	return nullptr;
}

Random ModuleResources::ImportFile(const char* file_assets, resource_type type)
{
	Random id;
	bool import_ok = false;
	std::string output_file;
	std::vector<Random> rand_vec;
	ResourceModel model;

	Resource* res = FindMeta(file_assets);
	if (res)
		return res->GetID();
	else
		RELEASE(res);

	id.SetNumber(Find(file_assets));

	if (id.GetNumber() != 0)
	{
		Resource* res = Get(id.GetNumber());
		res->Load();
		return id;
	}

	switch (type)
	{
	case resource_type::RESOURCE_TEXTURE_UI:
		App->importer->texture_imp->ImportTextureUI(file_assets, output_file);
		res = App->importer->texture_imp->Load(output_file.c_str());
		res->imported_file = output_file.c_str();
		return res->ID;
		break;
	case resource_type::RESOURCE_TEXTURE:
		res = App->importer->texture_imp->Load(file_assets);
		return res->ID;
		break;
	case resource_type::RESOURCE_MODEL:
		ImportAssets(file_assets);
		import_ok = model.ImportModel(file_assets, output_file, false, rand_vec, rand_vec);
		break;
	case resource_type::RESOURCE_MESH:
		res = App->importer->mesh_imp->Load(file_assets);
		return res->ID;
		break;
	}

	if (import_ok)
	{
		Resource* res = CreateNewResource(type);
		std::string file_name;
		App->fs->SplitFilePath(file_assets, nullptr, &file_name);
		res->file = file_name;

		char str_id[50];
		sprintf_s(str_id, 50, "%u", res->GetID().GetNumber());
		std::string path = LIBRARY_MODELS_FOLDER;
		path += str_id;
		path += ".model";

		bool ret = App->fs->CopyFromOutsideFS(output_file.c_str(), path.c_str());
		std::remove(output_file.c_str());
		res->imported_file = "." + path;
		id = res->ID;
		res->Load();

		SaveMeta(res->file.c_str(), res);
		SaveMeta(res->imported_file.c_str(), res);
	}

	return id;
}

bool ModuleResources::ImportMeta(const char* file_assets, const char* meta_path, Resource* res)
{
	bool import_ok = false;
	ResourceModel model;
	std::string output_file;

	Scene* meta = new Scene(meta_path);
	Scene meta_meshes = meta->GetArray("Meshes");
	Scene meta_textures = meta->GetArray("Textures");

	std::vector<Random> meshes;
	std::vector<Random> textures;

	for (uint i = 0;; i++)
	{
		if (meta_meshes.IsArraySection(i))
		{
			char name[30];
			sprintf_s(name, 30, "Mesh %i", i);
			uint32 mesh_id = meta_meshes.GetSectionArray(i).GetDouble(name);
			if(mesh_id != 0)
				meshes.push_back(mesh_id);

			sprintf_s(name, 30, "Texture %i", i);
			uint32 texture_id = meta_textures.GetSectionArray(i).GetDouble(name);
			if(texture_id != 0)
				textures.push_back(texture_id);
		}
		else
			break;
	}

	switch (res->type)
	{
	case RESOURCE_MODEL:
		import_ok = model.ImportModel(file_assets, output_file, true, meshes, textures);
		break;
	}

	if (import_ok)
	{
		char str_id[50];
		sprintf_s(str_id, 50, "%u", res->GetID().GetNumber());
		std::string path = LIBRARY_MODELS_FOLDER;
		path += str_id;
		path += ".model";

		bool ret = App->fs->CopyFromOutsideFS(output_file.c_str(), path.c_str());
		std::remove(output_file.c_str());
		res->imported_file = "." + path;
		res->Load();
	}

	RELEASE(meta);

	return import_ok;
}

bool ModuleResources::ImportAssets(const char* file)
{
	bool ret = false;

	std::string file_name;
	App->fs->SplitFilePath(file, nullptr, &file_name);

	if (!App->fs->IsInDirectory("/Assets/", file))
	{
		std::string dst = "/Assets/";
		dst += file_name;
		ret = App->fs->CopyFromOutsideFS(file, dst.c_str());
	}
	else
		ret = true;

	return ret;
}

Resource* ModuleResources::Get(uint32 id)
{
	std::map<uint32, Resource*>::iterator it_res = resources.find(id);
	if (it_res != resources.end())
		return it_res->second;

	return nullptr;
}

Resource* ModuleResources::CreateNewResource(resource_type type, uint32 id)
{
	Resource* res = nullptr;

	switch (type)
	{
	case resource_type::RESOURCE_MESH:
		res = (Resource*)new ResourceMesh(id);
		break;
	case resource_type::RESOURCE_TEXTURE:
		res = (Resource*)new ResourceTexture(id);
		break;
	case resource_type::RESOURCE_TEXTURE_UI:
		res = (Resource*)new ResourceTexture(id);
		res->type = RESOURCE_TEXTURE_UI;
		break;
	case resource_type::RESOURCE_MODEL:
		res = (Resource*)new ResourceModel(id);
		break;
	}

	if (res)
		resources[res->GetID().GetNumber()] = res;

	return res;
}

ResourceMesh * ModuleResources::ImportMesh(uint32 id)
{
	if (id == 0)
		return nullptr;

	Resource* resource_mesh = Get(id);

	if(resource_mesh)
		return (ResourceMesh*)resource_mesh;
	

	char str_id[50];
	sprintf_s(str_id, 50, "%u", id);
	std::string path = str_id;
	path += ".neko";
	Random new_id = ImportFile(path.c_str(), RESOURCE_MESH);

	resource_mesh = Get(new_id.GetNumber());

	return (ResourceMesh*)resource_mesh;
}

ResourceTexture* ModuleResources::ImportTexture(uint32 id)
{
	if (id == 0)
		return nullptr;

	Resource* resource_texture = Get(id);

	if (resource_texture)
		return (ResourceTexture*)resource_texture;

	char str_id[50];
	sprintf_s(str_id, 50, "%u", id);
	std::string path = LIBRARY_TEXTURES_FOLDER;
	path += str_id;
	path += ".dds";

	Random new_id = ImportFile(path.c_str(), RESOURCE_TEXTURE);

	resource_texture = Get(new_id.GetNumber());

	return (ResourceTexture*)resource_texture;
}

void ModuleResources::SaveMeta(const char * file, Resource* res)
{
	Scene* meta = new Scene();

	meta->AddInt("Type", res->type);
	meta->AddString("ExportedFile", res->imported_file.c_str());
	meta->AddDouble("ID", res->GetID().GetNumber());
	meta->AddString("File", res->file.c_str());

	std::string file_name, output = ".";
	output += ASSETS_META_FOLDER;

	switch (res->type)
	{
	case RESOURCE_MODEL: 
	{
		Scene meshes = meta->AddArray("Meshes");
		Scene textures = meta->AddArray("Textures");
		Scene* resource_model = new Scene(res->imported_file.c_str());
		Scene model_array = resource_model->GetArray("Model");

		for (int i = 0;; i++)
		{
			if (resource_model->IsArraySection(i))
			{
				Scene go = model_array.GetSectionArray(i);

				Scene mesh = meshes.AddSectionArray(i);
				Scene texture = textures.AddSectionArray(i);
				char name[20];
				//Mesh
				sprintf_s(name, 20, "Mesh %i", i);
				mesh.AddDouble(name, go.GetDouble("Mesh"));

				//Texture
				sprintf_s(name, 20, "Texture %i", i);
				texture.AddDouble(name, go.GetDouble("Texture"));
			}
			else
				break;
		}
		std::string extension;
		App->fs->SplitFilePath(file, nullptr, &file_name, &extension);
		output += file_name;

		RELEASE(resource_model);
	}
	break;
	case RESOURCE_MESH:
	{
		char id[50];
		uint32 m_id = res->GetID().GetNumber();
		sprintf_s(id, 50, "%u", m_id);
		output += id;
		ResourceMesh* mesh = (ResourceMesh*)res;
		meta->AddString("Name", mesh->name);
	}
		break;

	case RESOURCE_MESH_UI:
	{
		output += file;

		ResourceMesh* mesh = (ResourceMesh*)res;
		meta->AddString("Name", mesh->name);

	}
	break;

	case RESOURCE_TEXTURE:
	{
		char id[50];
		uint32 m_id = res->GetID().GetNumber();
		sprintf_s(id, 50, "%u", m_id);
		output += id;
	}
		break;

	case RESOURCE_TEXTURE_UI:
	{
		std::string extension;
		App->fs->SplitFilePath(file, nullptr, &file_name, &extension);
		output += file_name;
	}
	break;
	}
	
	output +=".meta";

	meta->Save(output.c_str());

	RELEASE(meta);
}
