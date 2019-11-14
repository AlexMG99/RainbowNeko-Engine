#include "Application.h"
#include "TextureImporter.h"
#include "ComponentMesh.h"
#include "Texture.h"

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

bool TextureImporter::Import(const char* path)
{
	//BROFILER_CATEGORY("ImportTexture_ModuleImporter", Profiler::Color::Gray);

	if (App->viewport->selected_object) {
		ComponentMesh* comp_mesh = App->viewport->selected_object->GetComponentMesh();
		ComponentTexture* comp_texture = App->viewport->selected_object->GetComponentTexture();
		Texture* texture;
		
		if(!comp_texture)
			comp_texture = new ComponentTexture(COMPONENT_TEXTURE, true, App->viewport->selected_object);

		//Load Texture
		std::string output_file, file, extension, texture_path = path;
		App->fs->SplitFilePath(texture_path.c_str(), nullptr, &file, &extension);

		if (extension == "dds" || extension == "DDS")
			texture = Load(path);
		else
		{
			App->fs->SplitFilePath(texture_path.c_str(), nullptr, &file, &extension);
			ImportTexture(std::string(file + "." + extension).c_str(), output_file);
			texture = Load(std::string("." + output_file).c_str());
		}

		comp_texture->AddTexture(texture);

		if (comp_mesh)
			comp_mesh->image_id = texture->image_id;
		else
		{
			LOG("The object does not have a MESH! Create one or select another object.");
			App->viewport->selected_object->DeleteComponent(comp_texture);
		}
	}
	else
	{
		LOG("Warning! Object no selected. Please, select an object.");
	}

	return true;
}

void TextureImporter::ImportTexture(const char* path, std::string& output_file)
{
	bool ret = true;
	std::string file_path = ASSETS_FOLDER + std::string(path);

	ILuint devil_id = 0;

	ilGenImages(1, &devil_id);
	ilBindImage(devil_id);
	ilLoadImage(file_path.c_str());

	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
		{
			std::string file, extension;
			App->fs->SplitFilePath(path, nullptr, &file, &extension);
			output_file = LIBRARY_TEXTURES_FOLDER + file + ".dds";
			App->fs->Save(output_file.c_str(), data, size);
		}
		RELEASE_ARRAY(data);
	}

	ilDeleteImages(1, &devil_id);
}


Texture* TextureImporter::Load(const char * file)
{
	bool ret = true;
	Texture* texture = new Texture();
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

	return texture;
}
