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
	std::string file_name, path = ".";
	path += ASSETS_META_FOLDER;
	App->fs->SplitFilePath(file, nullptr, &file_name);
	path += file_name.c_str();
	path += ".meta";

	Scene* meta = new Scene(path.c_str());

	if (meta->GetVRoot())
	{
		Resource* res = CreateNewResource(resource_type(meta->GetInt("Type")));
		res->ID.SetNumber(meta->GetDouble("ID"));
		res->file = meta->GetString("File");
		res->imported_file = meta->GetString("ExportedFile");
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
		import_ok = App->importer->texture_imp->Import(file_assets, output_file);
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
		res->imported_file = output_file;
		id = res->ID;
		res->Load();

		SaveMeta(std::string(res->file + ".meta").c_str(), res);
	}

	return id;
}

Random ModuleResources::GenerateNewID()
{
	return Random();
}

const Resource * ModuleResources::Get(Random id) const
{
	return nullptr;
}

Resource* ModuleResources::Get(uint32 id)
{
	std::map<uint32, Resource*>::iterator it_res = resources.find(id);
	if (it_res != resources.end())
		return it_res->second;

	return nullptr;
}

Resource* ModuleResources::CreateNewResource(resource_type type)
{
	Resource* res = nullptr;

	switch (type)
	{
	case resource_type::RESOURCE_MESH:
		res = (Resource*)new ResourceMesh();
		break;
	case resource_type::RESOURCE_TEXTURE:
		res = (Resource*)new ResourceTexture();
		break;
	case resource_type::RESOURCE_MODEL:
		res = (Resource*)new ResourceModel();
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

ResourceTexture* ModuleResources::ImportTexture(uint32 id, const char* path)
{
	if (id == 0)
		return nullptr;

	Resource* resource_texture = Get(id);

	if (resource_texture)
		return (ResourceTexture*)resource_texture;

	Random new_id = ImportFile(path, RESOURCE_TEXTURE);

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

	switch (res->type)
	{
	case RESOURCE_MODEL:
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
				char name[10];
				//Mesh
				uint32 m_id = go.GetDouble("Mesh");
				sprintf_s(id, 50, "%u", m_id);
				sprintf_s(name, 10, "Mesh %i", i);
				mesh.AddString(name, id);

				//Texture
				uint32 t_id = go.GetDouble("Texture");
				sprintf_s(id, 50, "%u", t_id);
				sprintf_s(name, 10, "Texture %i", i);
				texture.AddString(name, id);
			}
			else
				break;
		}
	}
	
	std::string file_name, output = ".";
	output += ASSETS_META_FOLDER;
	App->fs->SplitFilePath(file, nullptr, &file_name);
	output += file_name;

	bool ret = meta->Save(output.c_str());

}
