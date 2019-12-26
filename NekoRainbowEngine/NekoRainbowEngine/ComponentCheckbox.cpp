#include "Application.h"
#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

ComponentCheckbox::ComponentCheckbox(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char * path, bool state) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	comp_text->AddTexture(texture->CreateTexture("./Assets/checkbox.png"));
	my_go->GetComponentMesh()->image_id = texture->image_id;
}
