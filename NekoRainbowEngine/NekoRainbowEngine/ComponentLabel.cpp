#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentLabel.h"
#include "GL/include/glew.h"

ComponentLabel::ComponentLabel(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* text): ComponentUI(comp_type, act, obj, type, w, h, canvas, text)
{
	text_font = App->fonts->default_font;

	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	texture->SetSize(width, height);
	texture->image_id = text_font->Characters[text[0]].TextureID;
	my_go->GetComponentMesh()->image_id = texture->image_id;
	comp_text->AddTexture(texture);

	text_str = text;
}

ComponentLabel::~ComponentLabel()
{
}
