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
	path += LIBRARY_META_FOLDER;
	App->fs->SplitFilePath(file, nullptr, &file_name, &extension);
	path += file_name.c_str();
	path += ".meta";

	Scene* meta = new Scene(path.c_str());

	if (meta->GetVRoot())
	{
		Resource* res = CreateNewResource(resource_type(meta->GetInt("Type")), meta->GetDouble("ID"));
		res->file = meta->GetString("File");
		res->imported_file = meta->GetString("ExportedFile");
		if (res->type == RESOURCE_MESH)
		{
			ResourceMesh* mesh = (ResourceMesh*)res;
			mesh->name=meta->GetString("Name");
		}
			res->Load();

		return res;
	}

	return nullptr;
}

Random ModuleResources::ImportFile(const char* file_assets, resource_type type)
{
	Random id;
	bool import_ok = false;
	std::string output_file;
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
	case resource_type::RESOURCE_TEXTURE:
		res = App->importer->texture_imp->Load(file_assets);
		return res->ID;
		break;
	case resource_type::RESOURCE_MODEL:
		import_ok = model.ImportModel(file_assets, output_file);
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

Random ModuleResources::GenerateNewID()
{
	return Random();
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
	output += LIBRARY_META_FOLDER;

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
				char id[50];
				char name[20];
				//Mesh
				uint32 m_id = go.GetDouble("Mesh");
				sprintf_s(id, 50, "%u", m_id);
				sprintf_s(name, 20, "Mesh %i", i);
				mesh.AddString(name, id);

				//Texture
				uint32 t_id = go.GetDouble("Texture");
				sprintf_s(id, 50, "%u", t_id);
				sprintf_s(name, 20, "Texture %i", i);
				texture.AddString(name, id);
			}
			else
				break;
		}
		std::string extension;
		App->fs->SplitFilePath(file, nullptr, &file_name, &extension);
		output += file_name;
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

	case RESOURCE_TEXTURE:
	{
		char id[50];
		uint32 m_id = res->GetID().GetNumber();
		sprintf_s(id, 50, "%u", m_id);
		output += id;
	}
		break;
	}
	
	output +=".meta";

	bool ret = meta->Save(output.c_str());

}
