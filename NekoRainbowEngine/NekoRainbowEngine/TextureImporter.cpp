#include "Application.h"
#include "TextureImporter.h"
#include "ComponentMesh.h"

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
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool TextureImporter::Import(const char* path)
{
	//BROFILER_CATEGORY("ImportTexture_ModuleImporter", Profiler::Color::Gray);

	if (App->viewport->selected_object) {
		ComponentTexture* texture = App->viewport->selected_object->GetComponentTexture();
		ComponentMesh* mesh = App->viewport->selected_object->GetComponentMesh();

		if (!texture)
			texture = (ComponentTexture*)App->viewport->selected_object->CreateComponent(COMPONENT_TEXTURE);

		//Load Texture
		std::string file, extension, texture_path = path;
		App->fs->SplitFilePath(texture_path.c_str(), nullptr, &file, &extension);

		texture->LoadTexture(file.c_str());
		if (mesh)
			mesh->image_id = texture->image_id;
		else
		{
			LOG("The object does not have a MESH! Create one or select another object.");
			App->viewport->selected_object->DeleteComponent(texture);
		}
	}
	else
	{
		LOG("Warning! Object no selected. Please, select an object.");
	}
	return true;
}