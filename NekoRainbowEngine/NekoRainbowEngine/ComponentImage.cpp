#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentImage.h"
#include "TextureImporter.h"
#include "Scene.h"

ComponentImage::ComponentImage(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	img_path = path;
	panel.textureID = App->importer->texture_imp->GetImageID(path);
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

	Scene vertex = scene.AddSectionArray(0);
	Scene uv = scene.AddSectionArray(1);

	for (int i = 0; i < 4; i++)
	{
		char name[30];
		std::string num = "Vertex";
		num += std::to_string(i);
		strcpy_s(name, num.c_str());
		ret = vertex.AddFloat3(num.c_str(), panel.vertex[i]);

		num = "UV";
		num += std::to_string(i);
		strcpy_s(name, num.c_str());
		ret = vertex.AddFloat2(num.c_str(), panel.uv[i]);
	}

	ret = img_scene.AddInt("Texture", panel.textureID);

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
