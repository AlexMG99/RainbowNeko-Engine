#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentImage.h"

ComponentImage::ComponentImage(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = App->importer->ImportTexture(path);
	comp_text->AddTexture(texture);
	my_go->GetComponentMesh()->image_id = texture->image_id;
}
