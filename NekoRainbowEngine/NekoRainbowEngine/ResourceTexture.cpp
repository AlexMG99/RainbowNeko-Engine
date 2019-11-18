#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResources.h"
#include "ResourceTexture.h"

#include "Assimp/include/material.h"
#include "Assimp/include/types.h"
#include <string>

ResourceTexture::ResourceTexture() : Resource()
{
	this->type = RESOURCE_TEXTURE;
}

Random ResourceTexture::Import(const aiMaterial* texture, const char* path)
{
	std::string base_path;
	App->fs->SplitFilePath(path, &base_path, &file, nullptr);

	ResourceTexture* resource_texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));

	aiString texture_path;
	if(texture->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS);
	{
		std::string full_path = base_path.c_str();
		full_path += texture_path.C_Str();

		resource_texture->ID = App->resources->ImportFile(full_path.c_str(), resource_type::RESOURCE_TEXTURE);
	}

	return resource_texture->ID;
}