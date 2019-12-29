#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "CompomentCharacter.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentLabel.h"
#include "Scene.h"
#include "GL/include/glew.h"

ComponentLabel::ComponentLabel(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* text, uint x, uint y): ComponentUI(comp_type, act, obj, type, w, h, canvas, text, x, y)
{
	text_str = text;
	CreateText();
}

ComponentLabel::~ComponentLabel()
{
}

void ComponentLabel::CreateText()
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	uint width_ch = width / text_str.length();
	for (int i = 0; i < text_str.length(); i++)
	{
		uint pos_aux_x = pos_x + width_ch * i;
		GameObject* char_aux = App->viewport->CreateGameObject(std::string(text_str.substr(i,1) + "_") + text_str, my_go, float3(pos_aux_x, pos_y, 0));
		char_aux->CreateComponentUI(UI_Character, width_ch, height, canvas, text_str.substr(i, 1).c_str());
	}


}

bool ComponentLabel::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene label_scene = scene.AddSectionArray(i);

	ret = label_scene.AddInt("Type", type);
	ret = label_scene.AddString("Path", text_str);
	ret = label_scene.AddInt("UI_type", ui_type);
	ret = label_scene.AddInt("Width", width);
	ret = label_scene.AddInt("Height", height);
	ret = label_scene.AddInt("PosX", pos_x);
	ret = label_scene.AddInt("PosY", pos_y);

	return ret;
}

bool ComponentLabel::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene label_scene = scene.GetSectionArray(i);

	type = (component_type)label_scene.GetInt("Type");
	ui_type = (UI_type)label_scene.GetInt("UI_type");

	return ret;
}
