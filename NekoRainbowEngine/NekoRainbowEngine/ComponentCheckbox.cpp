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
	panel.textureID = App->importer->texture_imp->GetImageID("./Assets/checkbox.png");
	checkbox_true = App->importer->texture_imp->GetImageID("./Assets/checkboxOn.png");
}

bool ComponentCheckbox::OnClick()
{
	if (dragable)
		return true;

	//if (bool_state)
	//{
	//	my_go->GetComponentTexture()->AddTexture(checkbox_true);
	//	my_go->GetComponentMesh()->image_id = checkbox_true->image_id;
	//	bool_state = false;
	//}
	//else
	//{
	//	my_go->GetComponentTexture()->AddTexture(texture);
	//	my_go->GetComponentMesh()->image_id = texture->image_id;
	//	bool_state = true;
	//}

	return true;
}

void ComponentCheckbox::SetState(bool st)
{
	bool_state = &st;
}

bool ComponentCheckbox::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene check_scene = scene.AddSectionArray(i);

	ret = check_scene.AddInt("Type", type);
	ret = check_scene.AddInt("UI_type", ui_type);
	ret = check_scene.AddBool("Value", bool_state);

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


