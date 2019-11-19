#include "Application.h"
#include "TextureImporter.h"
#include "ComponentMesh.h"
#include "ResourceTexture.h"

//-------------- Devil --------------
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

bool TextureImporter::Init()
{
	ilInit();
	iluInit();
	ilutInit();

	return true;
}

bool TextureImporter::Import(const char* path, std::string& output_file)
{
	bool ret = false;

	ret = ImportTexture(path, output_file);

	return ret;
}

bool TextureImporter::ImportTexture(const char* path, std::string& output_file)
{
	bool ret = true;

	std::string file, extension;
	App->fs->SplitFilePath(path, nullptr, &file, &extension);

	ILuint devil_id = 0;
	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);

	if (ilLoadImage(path))
	{
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			{
				std::string path = LIBRARY_TEXTURES_FOLDER + output_file + ".dds";
				output_file = path;
				ret = App->fs->Save(output_file.c_str(), data, size);
			}
			RELEASE_ARRAY(data);
		}
		ilDeleteImages(1, &devil_id);
	}

	return ret;
}


ResourceTexture* TextureImporter::Load(const char * file)
{
	bool ret = true;
	ResourceTexture* texture = (ResourceTexture*)App->resources->CreateNewResource(RESOURCE_TEXTURE);
	ILuint devil_id = 0;

	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);
	ilutRenderer(ILUT_OPENGL);

	std::string path = file;
	App->fs->NormalizePath(path);

	if (!ilLoadImage(path.c_str())) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", file, ilGetString(error));
		ret = false;
	}
	else {
		texture->image_id = ilutGLBindTexImage();
		texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture->width = ilGetInteger(IL_IMAGE_WIDTH);

		texture->GenerateTexture();
		LOG("Loaded with path: %s succesfully!", file);
	}

	ilDeleteImages(1, &devil_id);
	texture->path = file;

	return texture;
}

bool TextureImporter::Load(ResourceTexture* texture)
{
	bool ret = true;
	ILuint devil_id = 0;

	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);
	ilutRenderer(ILUT_OPENGL);

	std::string path = ".";
	path.append(texture->imported_file.c_str());

	if (!ilLoadImage(path.c_str())) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", texture->imported_file.c_str(), ilGetString(error));
		ret = false;
	}
	else {
		texture->image_id = ilutGLBindTexImage();
		texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture->width = ilGetInteger(IL_IMAGE_WIDTH);

		texture->GenerateTexture();
		LOG("Loaded with path: %s succesfully!", texture->imported_file.c_str());
	}

	ilDeleteImages(1, &devil_id);

	return texture;
}