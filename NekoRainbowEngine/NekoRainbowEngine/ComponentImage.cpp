#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentImage.h"
#include "Scene.h"

ComponentImage::ComponentImage(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = App->importer->ImportTexture(path);
	comp_text->AddTexture(texture);
	my_go->GetComponentMesh()->image_id = texture->image_id;
}

bool ComponentImage::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene check_scene = scene.AddSectionArray(i);

	ret = check_scene.AddInt("Type", type);
	ret = check_scene.AddInt("UI_type", ui_type);
	ret = check_scene.AddDouble("Texture", texture->GetID().GetNumber());

	return ret;
}

bool ComponentImage::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene check_scene = scene.GetSectionArray(i);

	type = (component_type)check_scene.GetInt("Type");
	ui_type = (UI_type)check_scene.GetInt("UI_type");
	texture = App->resources->ImportTexture(check_scene.GetDouble("Texture"));

	my_go->GetComponentMesh()->image_id = texture->image_id;

	return ret;
}
