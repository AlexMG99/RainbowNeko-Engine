#include "Application.h"
#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "Scene.h"

ComponentCheckbox::ComponentCheckbox(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char * path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = App->importer->ImportTexture("./Assets/checkbox.png");
	comp_text->AddTexture(texture);
	my_go->GetComponentMesh()->image_id = texture->image_id;

	checkbox_true = App->importer->ImportTexture("./Assets/checkboxOn.png");
}

bool ComponentCheckbox::OnClick()
{
	if (bool_state)
	{
		my_go->GetComponentTexture()->AddTexture(checkbox_true);
		my_go->GetComponentMesh()->image_id = checkbox_true->image_id;
		bool_state = false;
	}
	else
	{
		my_go->GetComponentTexture()->AddTexture(texture);
		my_go->GetComponentMesh()->image_id = texture->image_id;
		bool_state = true;
	}

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
	ret = check_scene.AddDouble("CheckOff", texture->GetID().GetNumber());
	ret = check_scene.AddDouble("CheckOn", checkbox_true->GetID().GetNumber());
	ret = check_scene.AddBool("Value", bool_state);

	return ret;
}

bool ComponentCheckbox::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene check_scene = scene.AddSectionArray(i);

	type = (component_type)check_scene.GetInt("Type");
	ui_type = (UI_type)check_scene.GetInt("UI_type");
	bool_state = check_scene.GetBool("Value");
	texture = App->resources->ImportTexture(check_scene.GetDouble("CheckOff"));
	checkbox_true = App->resources->ImportTexture(check_scene.GetDouble("CheckOn"));

	my_go->GetComponentMesh()->image_id = texture->image_id;

	return ret;
}


