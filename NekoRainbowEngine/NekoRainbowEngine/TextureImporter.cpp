#include "Application.h"
#include "TextureImporter.h"
#include "ComponentMesh.h"
#include "ResourceTexture.h"
#include "GL/include/glew.h"

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

bool TextureImporter::ImportTextureUI(const char* path, std::string& output_file)
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
				std::string path = LIBRARY_TEXTURES_FOLDER + file + ".dds";
				output_file = path;
				ret = App->fs->Save(output_file.c_str(), data, size);
				output_file = ".";
				output_file += path;
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
	ResourceTexture* texture = (ResourceTexture*)App->resources->CreateNewResource(RESOURCE_TEXTURE_UI);
	ILuint devil_id = 0;

	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);
	ilutRenderer(ILUT_OPENGL);

	if (!ilLoadImage(file)) {
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
	texture->file = file;

	return texture;
}

bool TextureImporter::Load(ResourceTexture* texture)
{
	bool ret = true;
	ILuint devil_id = 0;

	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);
	ilutRenderer(ILUT_OPENGL);

	std::string path;
	if (texture->imported_file.at(0) != '.')
	{
		path = ".";
	}
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

uint TextureImporter::GetImageID(const char * path)
{
	uint image_id = 0;
	bool ret = true;
	ILuint devil_id = 0;

	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);
	ilutRenderer(ILUT_OPENGL);

	if (!ilLoadImage(path)) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
		ret = false;
	}
	else {
		image_id = ilutGLBindTexImage();

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

		LOG("Loaded with path: %s succesfully!", path);
	}

	ilDeleteImages(1, &devil_id);

	return image_id;

}
