#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"

update_status PanelHierarchy::Draw()
{
	ImGui::Begin(name, &enabled);

	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		ImGui::Selectable((*it_obj)->GetName().c_str(), &(*it_obj)->selected);
	}

	ImGui::End();
	return UPDATE_CONTINUE;
}
