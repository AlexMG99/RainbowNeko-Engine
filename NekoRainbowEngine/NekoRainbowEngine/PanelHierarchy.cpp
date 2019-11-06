#include "Brofiler/Brofiler.h"
#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"

update_status PanelHierarchy::Draw()
{
	BROFILER_CATEGORY("Draw_PanelHierarchy", Profiler::Color::GoldenRod);

	if (ImGui::Begin("Game Ojects", &enabled, ImGuiWindowFlags_AlwaysAutoResize))
	{
		node_it = 0;

		for(auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); it_obj++)
			TreeObject(*it_obj);

		ImGui::End();
	}

	return UPDATE_CONTINUE;
}

void PanelHierarchy::TreeObject(GameObject* obj)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static int node_num = -1;
	static int selection_mask = (1 << 2);
	node_it++;

	const bool is_selected = (selection_mask & (1 << node_it)) != 0;
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	ImGui::PushID(obj->GetId());
	//Check if has children
	if (obj->HasChildren())
	{	
		bool node_open = ImGui::TreeNodeEx(obj->GetName().c_str(), node_flags, obj->GetName().c_str());
		if (ImGui::IsItemClicked() && !is_selected) {
			node_num = node_it;
			App->viewport->selected_object = obj;
		}
		else if (ImGui::IsItemClicked() && is_selected) {
			node_num = -10;
			selection_mask = (1 << 2);
			App->viewport->selected_object = nullptr;
		}
		ImGui::PopID();
		if (node_open)
		{
			for (auto it_obj = obj->children.begin(); it_obj < obj->children.end(); it_obj++)
			{
				TreeObject(*it_obj);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx(obj->GetName().c_str(), node_flags, obj->GetName().c_str());
		if (ImGui::IsItemClicked() && !is_selected) {
			node_num = node_it;
			App->viewport->selected_object = obj;
		}
		else if (ImGui::IsItemClicked() && is_selected) {
			node_num = -10;
			selection_mask = (1 << 2);
			App->viewport->selected_object = nullptr;
		}
		ImGui::PopID();
	}

	if (node_num != -1)
	{
		selection_mask = (1 << node_num);
	}
}

