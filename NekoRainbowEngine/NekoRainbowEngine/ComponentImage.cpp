#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentImage.h"
#include "TextureImporter.h"
#include "Scene.h"

ComponentImage::ComponentImage(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas, const char* path) :ComponentUI(comp_type, act, obj, type, w, h, canvas, path)
{
	panel.textureID = App->importer->texture_imp->GetImageID(path);
}

bool ComponentImage::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene check_scene = scene.AddSectionArray(i);

	ret = check_scene.AddInt("Type", type);
	ret = check_scene.AddInt("UI_type", ui_type);

	return ret;
}

bool ComponentImage::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene check_scene = scene.GetSectionArray(i);

	type = (component_type)check_scene.GetInt("Type");
	ui_type = (UI_type)check_scene.GetInt("UI_type");

	return ret;
}
