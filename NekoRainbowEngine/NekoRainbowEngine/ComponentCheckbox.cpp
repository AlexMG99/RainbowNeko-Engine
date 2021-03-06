#include "Application.h"
#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "TextureImporter.h"
#include "Scene.h"

ComponentCheckbox::ComponentCheckbox(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char * path, uint x, uint y) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path, x, y)
{
	checkbox_name = path;
	panel_in_game.textureID = panel_in_scene.textureID = checkbox_false = App->importer->texture_imp->GetImageID("./Assets/checkbox.png");
	checkbox_true = App->importer->texture_imp->GetImageID("./Assets/checkboxOn.png");
}

bool ComponentCheckbox::OnClick()
{
	if (dragable && checkbox_name != "Draggable")
		return true;

	if (bool_state)
	{
		panel_in_scene.textureID = checkbox_false;
		panel_in_game.textureID = checkbox_false;
		bool_state = false;
	}
	else
	{
		panel_in_scene.textureID = checkbox_true;
		panel_in_game.textureID = checkbox_true;
		bool_state = true;
	}

	if (checkbox_name == "Draggable")
	{
		SetDragable();
		LOG("Draggable set to: %i", bool_state);
	}
	else if (checkbox_name == "Vsync")
	{
		App->window->SetVsync(bool_state);
		LOG("Vsync set to: %i", bool_state);
	}

	return true;
}

void ComponentCheckbox::SetState(bool st)
{
	if (st)
	{
		panel_in_game.textureID = panel_in_scene.textureID = checkbox_true;
	}
	else
	{
		panel_in_game.textureID = panel_in_scene.textureID = checkbox_false;
	}

	bool_state = &st;
}

bool ComponentCheckbox::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene check_scene = scene.AddSectionArray(i);

	ret = check_scene.AddInt("Type", type);
	ret = check_scene.AddString("Path", checkbox_name);
	ret = check_scene.AddInt("UI_type", ui_type);
	ret = check_scene.AddInt("Width", width);
	ret = check_scene.AddInt("Height", height);
	ret = check_scene.AddInt("PosX", pos_x);
	ret = check_scene.AddInt("PosY", pos_y);

	return ret;
}

bool ComponentCheckbox::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene check_scene = scene.GetSectionArray(i);

	type = (component_type)check_scene.GetInt("Type");
	ui_type = (UI_type)check_scene.GetInt("UI_type");
	bool_state = check_scene.GetBool("Value");

	return ret;
}

void ComponentCheckbox::SetDragable()
{
	for (auto it_go = canvas->my_go->children.begin(); it_go != canvas->my_go->children.end(); it_go++)
	{
		if ((*it_go)->GetComponentUI())
			(*it_go)->GetComponentUI()->dragable = bool_state;
	}
}




