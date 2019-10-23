#include "Brofiler/Brofiler.h"
#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"


update_status PanelHierarchy::Draw()
{

	BROFILER_CATEGORY("Draw_PanelHierarchy", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled);

	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		if(ImGui::Selectable((*it_obj)->GetName().c_str(), &(*it_obj)->selected))
		{
			App->viewport->CheckObjectSelected((*it_obj));
		}
	}

	ImGui::End();
	//bool ret = true;
	//if (ImGui::Begin("Game Ojects", &ret, ImGuiWindowFlags_AlwaysAutoResize))
	//{
	//	//show a list of game objects
	//	ImGui::LabelText("", "Game Objects in Scene.");

	//	if (ImGui::TreeNode("Object List"))
	//	{
	//		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);

	//		TreeObject(App->viewport->root_object);

	//		ImGui::TreePop();
	//		ImGui::PopStyleVar();
	//	}
	//	ImGui::End();
	//}

	return UPDATE_CONTINUE;
}

void PanelHierarchy::TreeObject(GameObject* obj)
{
	for (auto it_obj = obj->children.begin(); it_obj < obj->children.end(); it_obj++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (ImGui::TreeNodeEx((*it_obj)->GetName().c_str(), node_flags, (*it_obj)->GetName().c_str()))
		{
			App->viewport->CheckObjectSelected((*it_obj));
		}
		TreeObject(*it_obj);
	}
}

