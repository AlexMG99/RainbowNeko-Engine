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
	std::string file_str(file);
	App->fs->NormalizePath(file_str);

	for (std::map<uint32, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->file.compare(file_str) == 0)
			return it->first;
	}

	return 0;
}

Random ModuleResources::ImportFile(const char* file_assets, resource_type type)
{
	Random id;
	bool import_ok = false;
	std::string output_file;
	ResourceModel model;

	id.SetNumber(Find(file_assets));

	if (id.GetNumber() != 0)
		return id;

	switch (type)
	{
	case resource_type::RESOURCE_TEXTURE:
		import_ok = App->importer->texture_imp->Import(file_assets, output_file);
		break;
	case resource_type::RESOURCE_MODEL:
		import_ok = model.ImportModel(file_assets, output_file);
		break;
	case resource_type::RESOURCE_MESH:
		import_ok = App->importer->mesh_imp->Load(file_assets);
		break;
	}

	if (import_ok)
	{
		Resource* res = CreateNewResource(type);
		res->file = file_assets;
		res->imported_file = output_file;
		id = res->ID;
		res->Load();
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

ResourceMesh * ModuleResources::ImportMesh(uint32 id, const char* path)
{
	if (id == 0)
		return nullptr;

	Resource* resource_mesh = Get(id);

	if(resource_mesh)
		return (ResourceMesh*)resource_mesh;
	
	Random new_id = ImportFile(path, RESOURCE_MESH);

	resource_mesh = Get(new_id.GetNumber());

	return (ResourceMesh*)resource_mesh;
}
