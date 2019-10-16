#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"
#include "imgui/imgui_docking.h"

update_status PanelHierarchy::Draw()
{
	ImGui::BeginDock("Hierarchy", false, &visible, false);
	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		ImGui::Selectable((*it_obj)->GetName().c_str(), &(*it_obj)->selected);
	}

	ImGui::EndDock();
	return UPDATE_CONTINUE;
}
