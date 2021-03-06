#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResources.h"
#include "ResourceTexture.h"
#include "TextureImporter.h"
#include "Scene.h"

#include "GL/include/glew.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#include "Assimp/include/material.h"
#include "Assimp/include/types.h"
#include <string>

ResourceTexture::ResourceTexture() : Resource()
{
	this->type = RESOURCE_TEXTURE;
}

ResourceTexture::ResourceTexture(uint32 id) : Resource(id, type)
{
	type = resource_type::RESOURCE_TEXTURE;
}

Random ResourceTexture::Import(const aiMaterial* texture, const char* path)
{
	std::string base_path;
	App->fs->SplitFilePath(path, &base_path, &file, nullptr);

	ResourceTexture* resource_texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));

	aiString texture_path;
	if(texture->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS);
	{
		std::string file_path, full_path = base_path.c_str();
		App->fs->SplitFilePath(texture_path.C_Str(), nullptr, &file_path, nullptr);
		if (file_path.empty())
			file_path = "default_texture.png";
		full_path += file_path;

		App->resources->ImportAssets(full_path.c_str());

		char id[15];
		sprintf_s(id, 15, "%u", resource_texture->GetID().GetNumber());
		std::string output = id;
		App->importer->texture_imp->Import(full_path.c_str(), output);
		resource_texture->imported_file = output;

		resource_texture->Load();

		App->resources->SaveMeta(texture_path.C_Str(), resource_texture);
	}

	return resource_texture->ID;
}

void ResourceTexture::Import(const aiMaterial* texture, const char* path, Random text_id)
{
	std::string base_path;
	App->fs->SplitFilePath(path, &base_path, &file, nullptr);

	ResourceTexture* resource_texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE, text_id.GetNumber()));

	aiString texture_path;
	if (texture->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS);
	{
		std::string file_path, full_path = base_path.c_str();
		App->fs->SplitFilePath(texture_path.C_Str(), nullptr, &file_path, nullptr);
		full_path += file_path;

		App->resources->ImportAssets(full_path.c_str());

		char id[15];
		sprintf_s(id, 15, "%u", resource_texture->GetID().GetNumber());
		std::string output = id;
		App->importer->texture_imp->Import(full_path.c_str(), output);
		resource_texture->imported_file = output;

		resource_texture->Load();
	}
}

void ResourceTexture::GenerateTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, image_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

	glBindTexture(GL_TEXTURE_2D, 0);

}

bool ResourceTexture::Load()
{
	return App->importer->texture_imp->Load(this);
}

ResourceTexture* ResourceTexture::Load(Scene& scene)
{
	return App->resources->ImportTexture(scene.GetDouble("Texture"));
}

ResourceTexture * ResourceTexture::CreateTexture(const char * path)
{
	char id[15];
	sprintf_s(id, 15, "%u", GetID().GetNumber());
	std::string output = id;
	App->importer->texture_imp->Import(path, output);
	imported_file = output;

	Load();

	return this;
}

void ResourceTexture::SetSize(uint w, uint h)
{
	width = w;
	height = h;
}
