#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentImage.h"
#include "TextureImporter.h"
#include "Scene.h"

ComponentImage::ComponentImage(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* path, uint x, uint y) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path, x, y)
{
	img_path = path;
	panel_in_game.textureID = panel_in_scene.textureID = App->importer->texture_imp->GetImageID(path);
}

bool ComponentImage::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene img_scene = scene.AddSectionArray(i);

	ret = img_scene.AddInt("Type", type);
	ret = img_scene.AddString("Path", img_path);
	ret = img_scene.AddInt("UI_type", ui_type);
	ret = img_scene.AddInt("Width", width);
	ret = img_scene.AddInt("Height", height);
	ret = img_scene.AddInt("PosX", pos_x);
	ret = img_scene.AddInt("PosY", pos_y);

	Scene vertex = scene.AddSectionArray(0);

	for (int i = 0; i < 4; i++)
	{
		char name[30];
		std::string num = "Vertex";
		num += std::to_string(i);
		strcpy_s(name, num.c_str());
		ret = vertex.AddFloat3(num.c_str(), panel_in_scene.vertex[i]);
	}

	ret = img_scene.AddInt("Texture", panel_in_scene.textureID);

	return ret;
}

bool ComponentImage::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene img_scene = scene.GetSectionArray(i);

	type = (component_type)img_scene.GetInt("Type");
	ui_type = (UI_type)img_scene.GetInt("UI_type");


	return ret;
}
