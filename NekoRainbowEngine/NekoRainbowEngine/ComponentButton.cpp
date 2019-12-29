#include "Application.h"
#include "ComponentCanvas.h"
#include "ModuleResources.h"
#include "ComponentButton.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "TextureImporter.h"
#include "GL/include/glew.h"
#include "Scene.h"

ComponentButton::ComponentButton(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path, x, y)
{
	img_path = path;
	panel.textureID = App->importer->texture_imp->GetImageID(path);
}

bool ComponentButton::UpdateUI(float dt)
{
	if (function)
	{
		function = !FadeToBlack(dt);

		/*if (!function)
			App->viewport->to_load_scene01 = true;*/
	}

	return true;
}

bool ComponentButton::OnHover()
{
	fill_color = vec4(0.5, 0.5, 0.5, fill_color.w);
	return true;
}

bool ComponentButton::OnClick()
{
	fill_color = vec4(0.2, 0.2, 0.2, fill_color.w);
	function = true;
	return true;
}

bool ComponentButton::OnSave(Scene & scene, int i) const
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
		ret = vertex.AddFloat3(num.c_str(), panel.vertex[i]);
	}

	ret = img_scene.AddInt("Texture", panel.textureID);

	return ret;
}

bool ComponentButton::OnLoad(Scene & scene, int i)
{
	return true;
}

bool ComponentButton::FadeToBlack(float dt)
{
	bool ret = false;

	for (auto it_go = canvas->my_go->children.begin(); it_go != canvas->my_go->children.end(); it_go++)
	{
		ret = (*it_go)->GetComponentUI()->Fade();
	}
	
	return ret;

}

