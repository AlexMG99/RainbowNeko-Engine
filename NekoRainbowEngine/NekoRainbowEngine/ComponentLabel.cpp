#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "CompomentCharacter.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentLabel.h"
#include "GL/include/glew.h"

ComponentLabel::ComponentLabel(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* text): ComponentUI(comp_type, act, obj, type, w, h, canvas, text)
{
	text_str = text;
	CreateText();
}

ComponentLabel::~ComponentLabel()
{
}

void ComponentLabel::CreateText()
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	uint width_ch = width / text_str.length();
	for (int i = 0; i < text_str.length(); i++)
	{
		uint pos_aux_x = width_ch * (text_str.length() - i);
		GameObject* char_aux = App->viewport->CreateGameObject(text_str.substr(i,1), my_go, float3(pos_aux_x, 0, 0));
		char_aux->CreateComponentUI(UI_Character, width_ch, height, canvas, text_str.substr(i, 1).c_str());
	}


}
