#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResources.h"
#include "ResourceTexture.h"

#include "Assimp/include/material.h"
#include "Assimp/include/types.h"

Random ResourceTexture::Import(const aiMaterial* texture, const char* path)
{
	std::string base_path;
	App->fs->SplitFilePath(path, &base_path, nullptr, nullptr);

	ResourceTexture* resource_texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));

	aiString texture_path;
	if(texture->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS);
	{
		aiString full_path(base_path);
		full_path.Append(file.data);

		resource_texture->ID = App->resources->ImportFile(full_path.C_Str(), resource_type::RESOURCE_TEXTURE);
	}
}