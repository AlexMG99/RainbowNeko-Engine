#include "Application.h"
#include "GL/include/glew.h"
#include "PanelInspector.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentMesh.h"
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
		ImGui::Text("Name: %s", object->GetName().c_str()); ImGui::Separator();
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
			ImGui::Separator();
		}

		//Mesh
		ComponentMesh* comp_mesh = object->GetComponentMesh();
		if (comp_mesh)
		{
			if (ImGui::CollapsingHeader("Mesh")) 
			{
				ImGui::Text("Id vertices: %i", comp_mesh->id_vertex);
				ImGui::Text("Num vertices: %i", comp_mesh->num_vertices);
				ImGui::Text("Id indices: %i", comp_mesh->id_index);
				ImGui::Text("Num indices: %i", comp_mesh->num_index);
				ImGui::Text("Id uv: %i", comp_mesh->uv_id);
			}
		}

		//Texture
		ComponentTexture* comp_texture = object->GetComponentTexture();
		if (comp_texture)
		{
			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Text("Id texture: %i", comp_texture->image_id);
				ImGui::Image((ImTextureID)comp_texture->image_id, ImVec2(200, 200));
			}
		}
	}

	ImGui::End();

	return UPDATE_CONTINUE;
}
