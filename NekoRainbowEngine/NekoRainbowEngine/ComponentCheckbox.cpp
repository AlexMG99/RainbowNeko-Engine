#include "Application.h"
#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

ComponentCheckbox::ComponentCheckbox(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char * path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	comp_text->AddTexture(texture->CreateTexture("./Assets/checkbox.png"));
	my_go->GetComponentMesh()->image_id = texture->image_id;

	checkbox_true = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	checkbox_true->CreateTexture("./Assets/checkboxOn.png");

}

bool ComponentCheckbox::OnClick()
{
	if (state)
	{
		my_go->GetComponentTexture()->AddTexture(checkbox_true);
		my_go->GetComponentMesh()->image_id = checkbox_true->image_id;
		state = false;
	}
	else
	{
		my_go->GetComponentTexture()->AddTexture(texture);
		my_go->GetComponentMesh()->image_id = texture->image_id;
		state = true;
	}

	return true;
}

void ComponentCheckbox::SetState(bool st)
{
	state = &st;
}
