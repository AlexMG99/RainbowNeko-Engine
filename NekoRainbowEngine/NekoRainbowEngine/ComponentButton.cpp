#include "Application.h"
#include "ComponentCanvas.h"
#include "ModuleResources.h"
#include "ComponentButton.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "GL/include/glew.h"

ComponentButton::ComponentButton(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	comp_text->AddTexture(texture->CreateTexture("./Assets/button.png")); //TODO: Add custom buttton image
	my_go->GetComponentMesh()->image_id = texture->image_id;
}

bool ComponentButton::UpdateUI(float dt)
{
	if (function)
	{
		function = !FadeToBlack(dt);

		if (!function)
			App->viewport->to_load_scene01 = true;
	}

	return true;
}

bool ComponentButton::OnHover()
{
	my_go->GetComponentMesh()->ChangeColor(vec4(0.5, 0.5, 0.5, 1));
	return true;
}

bool ComponentButton::OnClick()
{
	my_go->GetComponentMesh()->ChangeColor(vec4(0.2, 0.2, 0.2, 1));
	function = true;
	return true;
}

bool ComponentButton::FadeToBlack(float dt)
{
	bool ret = false;

	for (auto it_go = canvas->my_go->children.begin(); it_go != canvas->my_go->children.end(); it_go++)
	{
		ret = (*it_go)->GetComponentMesh()->Fade(dt);
	}
	
	return ret;

}

