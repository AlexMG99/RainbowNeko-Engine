#include "Application.h"
#include "ComponentCanvas.h"
#include "ModuleResources.h"
#include "ComponentButton.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "TextureImporter.h"
#include "GL/include/glew.h"

ComponentButton::ComponentButton(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	panel.textureID = App->importer->texture_imp->GetImageID(path);
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
	LOG("ONHOVER");
	fill_color = vec4(0.5, 0.5, 0.5, 1);
	return true;
}

bool ComponentButton::OnClick()
{
	fill_color = vec4(0.2, 0.2, 0.2, 1);
	function = true;
	return true;
}

bool ComponentButton::FadeToBlack(float dt)
{
	bool ret = false;

	for (auto it_go = canvas->my_go->children.begin(); it_go != canvas->my_go->children.end(); it_go++)
	{
		if((*it_go)->GetComponentMesh())
			ret = (*it_go)->GetComponentMesh()->Fade(dt);
	}
	
	return ret;

}

