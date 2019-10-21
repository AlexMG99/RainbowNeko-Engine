#include "Application.h"
#include "GL/include/glew.h"
#include "PanelInspector.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "imgui/imgui.h"
#include "MathGeoLib/include/Math/Quat.h"

#include <list>

update_status PanelInspector::Draw()
{
	ImGui::Begin(name, &enabled);

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
		if (comp_trans && ImGui::CollapsingHeader("Transform"))
		{
			//Position / Rotation / Scale
			ImGui::InputFloat3("Position", comp_trans->position, 2, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("Scale", comp_trans->scale, 2, ImGuiInputTextFlags_ReadOnly);
			float angle[3] = { comp_trans->rotation.x ,comp_trans->rotation.y, comp_trans->rotation.z };
			ImGui::InputFloat3("Rotation", angle, 2, ImGuiInputTextFlags_ReadOnly);
			ImGui::Separator();
		}

		//Mesh
		ComponentMesh* comp_mesh = object->GetComponentMesh();
		if (comp_mesh && ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::PushID("Mesh");
			ImGui::Checkbox("Active", &comp_mesh->active);
			ImGui::Text("Id vertices: %i", comp_mesh->id_vertex);
			ImGui::Text("Num vertices: %i", comp_mesh->num_vertices);
			ImGui::Text("Id indices: %i", comp_mesh->id_index);
			ImGui::Text("Num indices: %i", comp_mesh->num_index);
			ImGui::Text("Id uv: %i", comp_mesh->uv_id);
			ImGui::PopID();
			ImGui::Separator();
		}

		//Texture
		ComponentTexture* comp_texture = object->GetComponentTexture();
		if (comp_texture)
		{
			if (ImGui::CollapsingHeader("Texture")) 
			{
				ImGui::PushID("Texture");
				ImGui::Checkbox("Active", &comp_texture->active);
				ImGui::Text("Path: %s", comp_texture->path.c_str());
				ImGui::Text("Id texture: %i", comp_texture->image_id);
				ImGui::Text("W: %i		H: %i", comp_texture->width, comp_texture->height);
				ImGui::Image((ImTextureID)comp_texture->image_id, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::PopID();
				ImGui::Separator();
			}
		}

		//Shape
		ComponentShape* comp_shape = object->GetComponentShape();
		if (comp_shape)
		{
			if (ImGui::CollapsingHeader("Shape"))
			{
				ImGui::Text("Type: %s", comp_shape->GetTypeString().c_str());
				int division[2] = { comp_shape->stack, comp_shape->slice };
				ImGui::InputInt2("Divisions", division, ImGuiInputTextFlags_ReadOnly);

			}
		}
	}
	ImGui::End();
	return UPDATE_CONTINUE;
}
