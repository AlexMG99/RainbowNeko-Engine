#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentLabel.h"
#include "ModuleFonts.h"
#include "GL/include/glew.h"
#include "SDL2_ttf/include/SDL_ttf.h"

ComponentLabel::ComponentLabel(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas): ComponentUI(comp_type, act, obj, type, w, h, canvas)
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	comp_trans->local_position = { comp_trans->local_position.x, comp_trans->local_position.y + canvas->height, comp_trans->local_position.z };
	comp_trans->GetGlobalTransformMatrix();
	font = App->fonts->LoadFont(DEFAULT_FONT, 48);

	ComponentMesh* comp_mesh = (ComponentMesh*)my_go->CreateComponent(COMPONENT_MESH);

	float3* vertex = new float3[4];
	vertex[0] = float3(0, 0, 0);
	vertex[1] = float3(width, 0, 0);
	vertex[2] = float3(0, height, 0);
	vertex[3] = float3(width, height, 0);

	mesh = new ResourceMesh();
	comp_mesh->AddMesh(mesh->CreateMesh(vertex));

	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	texture->SetSize(width, height);
	comp_text->AddTexture(texture);

	text_str = "Text";

}

ComponentLabel::~ComponentLabel()
{
	TTF_CloseFont(font->font);
	RELEASE(font);
}

bool ComponentLabel::CreateText()
{
	if (text_str.empty())
		return false;
}

void ComponentLabel::UpdateText()
{
	/*TTF_SizeText(font->font, text_str.c_str(), &width, &height);
	
	text_surface = TTF_RenderText_Blended_Wrapped(font->font, text_str.c_str(), SDL_Color{ 255,255,255 }, width);
	
	glBindTexture(GL_TEXTURE_2D, texture->image_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, text_surface->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	my_go->GetComponentMesh()->image_id = texture->image_id;
	SDL_FreeSurface(text_surface);*/

}

bool ComponentLabel::Update()
{
	if (!update_text)
	{
		UpdateText();
		update_text = true;
	}
	return true;
}

void ComponentLabel::Draw()
{

}
