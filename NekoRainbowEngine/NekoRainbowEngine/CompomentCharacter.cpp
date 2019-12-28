#include "Application.h"
#include "ModuleFonts.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceTexture.h"
#include "CompomentCharacter.h"

ComponentCharacter::ComponentCharacter(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	text_font = App->fonts->default_font;
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	texture->SetSize(width, height);
	texture->image_id = text_font->Characters.at(path[0]).TextureID;
	my_go->GetComponentMesh()->image_id = texture->image_id;
	comp_text->AddTexture(texture);
}
