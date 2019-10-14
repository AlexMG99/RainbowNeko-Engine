#include "Application.h"
#include "PanelInspector.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "imgui/imgui.h"

#include <list>

update_status PanelInspector::Draw()
{
	ImGui::Begin(name);

	GameObject* object = nullptr;

	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		if ((*it_obj)->selected)
			object = (*it_obj);

	}

	
	if (object)
	{
		//Transform
		ImGui::Text("Name: %s", object->GetName().c_str());
		ComponentTransform* comp_trans = object->GetComponentTransform();
		if (comp_trans)
		{
			//Position / Rotation / Scale
			if(ImGui::CollapsingHeader("Transform")) 
			{
				ImGui::InputFloat3("Position", comp_trans->position, 2, ImGuiInputTextFlags_ReadOnly);
				ImGui::InputFloat3("Scale", comp_trans->scale, 2, ImGuiInputTextFlags_ReadOnly);
				//ImGui::InputFloat3("Rotation", comp_trans->rotation, 2, ImGuiInputTextFlags_ReadOnly);
			}
		}

	}

	ImGui::End();

	return UPDATE_CONTINUE;
}
