#include "Brofiler/Brofiler.h"
#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"


update_status PanelHierarchy::Draw()
{

	BROFILER_CATEGORY("Draw_PanelHierarchy", Profiler::Color::GoldenRod);

	bool ret = true;
	if (ImGui::Begin("Game Ojects", &ret, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (ImGui::TreeNode("Object List"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);

			TreeObject(App->viewport->root_object);

			ImGui::TreePop();
			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	return UPDATE_CONTINUE;
}

void PanelHierarchy::TreeObject(GameObject* obj)
{
	bool pop = false;
	static uint i = 0;
	static uint node_num = -1;
	static bool is_selected = false;

	for (auto it_obj = obj->children.begin(); it_obj < obj->children.end(); it_obj++)
	{
		ImGuiTreeNodeFlags node_flags;
		i++;
		if (is_selected && (node_num == i))
		{
			App->viewport->selected_object = (*it_obj);
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}
		else
			node_flags = node_flags & ~ImGuiTreeNodeFlags_Selected;

		//Check if has children

		if ((*it_obj)->HasChildren())
		{
			node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			pop = true;
		}
		else
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			pop = false;
		}

		//Create Tree Node elements: Checks if has more children ? calls TreeObject : prints Node
		if (pop) {
			
			bool node_open = ImGui::TreeNodeEx((*it_obj)->GetName().c_str(), node_flags, (*it_obj)->GetName().c_str());
			
			if (node_open)
			{
				TreeObject(*it_obj);
				ImGui::TreePop();
			}
		}
		else
			ImGui::TreeNodeEx((*it_obj)->GetName().c_str(), node_flags, (*it_obj)->GetName().c_str());

		// Set Selectable
		if (ImGui::IsItemClicked())
		{
			is_selected = !is_selected;
			node_num = i;
		}

		
	}
	i = 0;
}

