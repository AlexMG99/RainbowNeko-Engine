#include "Application.h"
#include "ModuleFonts.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceTexture.h"
#include "CompomentCharacter.h"
#include "Scene.h"

ComponentCharacter::ComponentCharacter(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path, x, y)
{
	panel.textureID = text_font->Characters.at(path[0]).TextureID;
}

bool ComponentCharacter::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene char_scene = scene.AddSectionArray(i);

	ret = char_scene.AddInt("Type", type);
	ret = char_scene.AddInt("UI_type", ui_type);

	return ret;
}

bool ComponentCharacter::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene char_scene = scene.GetSectionArray(i);

	type = (component_type)char_scene.GetInt("Type");
	ui_type = (UI_type)char_scene.GetInt("UI_type");

	return ret;
}
