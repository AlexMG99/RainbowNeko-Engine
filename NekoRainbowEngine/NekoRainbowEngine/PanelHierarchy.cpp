#include "Brofiler/Brofiler.h"
#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include "imgui/imgui_internal.h"

update_status PanelHierarchy::Draw()
{
	BROFILER_CATEGORY("Draw_PanelHierarchy", Profiler::Color::GoldenRod);

	if (ImGui::Begin("Panel Hierarchy", &enabled, ImGuiWindowFlags_AlwaysAutoResize))
	{
		node_it = 0;

		for (int i = 0; i < App->viewport->root_object->children.size(); i++)
			TreeObject(App->viewport->root_object->children[i]);

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

		BeginDragDropSource(obj);
		BeginDragDropTarget(obj);
		BeginDragDropTargetRoot(obj);

		if (ImGui::IsItemClicked() && !is_selected) {
			node_num = node_it;
			if (App->viewport->selected_object)
				App->viewport->selected_object->SetSelected(false);
			App->viewport->selected_object = obj;
			App->viewport->selected_object->SetSelected(true);
		}
		else if (ImGui::IsItemClicked() && is_selected) {
			node_num = -10;
			selection_mask = (1 << 2);
			if (App->viewport->selected_object)
				App->viewport->selected_object->SetSelected(false);
			App->viewport->selected_object = nullptr;
		}

		ImGui::PopID();

		if (node_open)
		{
			for (int i = 0; i < obj->children.size(); i++)
				TreeObject(obj->children[i]);

			ImGui::TreePop();
		}
	}
	else
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx(obj->GetName().c_str(), node_flags, obj->GetName().c_str());

		BeginDragDropSource(obj);
		BeginDragDropTarget(obj);

		if (ImGui::IsItemClicked() && !is_selected) {
			node_num = node_it;
			if(App->viewport->selected_object)
				App->viewport->selected_object->SetSelected(false);
			App->viewport->selected_object = obj;
			App->viewport->selected_object->SetSelected(true);
		}
		else if (ImGui::IsItemClicked() && is_selected) {
			node_num = -10;
			selection_mask = (1 << 2);
			if (App->viewport->selected_object)
				App->viewport->selected_object->SetSelected(false);
			App->viewport->selected_object = nullptr;
		}
		ImGui::PopID();
	}

	if (node_num != -1)
	{
		selection_mask = (1 << node_num);
	}
}

void PanelHierarchy::BeginDragDropSource(GameObject* obj)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("HierarchyD&D", &obj, sizeof(GameObject));
		ImGui::EndDragDropSource();
	}
}

void PanelHierarchy::BeginDragDropTarget(GameObject* obj)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyD&D"))
		{
			GameObject* new_obj = *(GameObject**)payload->Data;

			if (!new_obj->IsChild(obj) && (new_obj != obj))
			{
				new_obj->GetParent()->RemoveChild(new_obj);
				new_obj->SetParent(obj);

				ComponentTransform* trans = new_obj->GetComponentTransform();
				if (trans)
					trans->UpdateComponents();
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void PanelHierarchy::BeginDragDropTargetRoot(GameObject* obj)
{
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), (ImGuiID)"Panel Hierarchy"))
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyD&D"))
		{
			GameObject* new_obj = *(GameObject**)payload->Data;

			if (!App->viewport->root_object->IsDirectChild(new_obj))
			{
				new_obj->GetParent()->RemoveChild(new_obj);
				new_obj->SetParent(App->viewport->root_object);

				ComponentTransform* trans = new_obj->GetComponentTransform();
				if (trans)
					trans->UpdateComponents();
				
			}
		}
		ImGui::EndDragDropTarget();
	}
}


