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
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	comp_trans->local_position = { comp_trans->local_position.x, comp_trans->local_position.y + canvas->height, comp_trans->local_position.z };
	comp_trans->GetGlobalTransformMatrix();

	text_font = App->fonts->default_font;

	ComponentMesh* comp_mesh = (ComponentMesh*)my_go->CreateComponent(COMPONENT_MESH);

	float3* vertex = new float3[4];
	vertex[0] = float3(0, height, 0);
	vertex[1] = float3(width, height, 0);
	vertex[3] = float3(width, 0, 0);
	vertex[2] = float3(0, 0, 0);

	mesh = new ResourceMesh();
	comp_mesh->AddMesh(mesh->CreateMesh(vertex));

	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	texture->SetSize(width, height);
	texture->image_id = text_font->Characters[text[0]].TextureID;
	comp_mesh->image_id = texture->image_id;
	comp_text->AddTexture(texture);

	text_str = text;
}

ComponentLabel::~ComponentLabel()
{
}
