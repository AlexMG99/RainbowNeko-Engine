#include "Application.h"
#include "GL/include/glew.h"
#include "PanelInspector.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "imgui/imgui.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Brofiler/Brofiler.h"
#include <list>

update_status PanelInspector::Draw()
{

	BROFILER_CATEGORY("Draw_PaneInspector", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled);

	GameObject* object = App->viewport->selected_object;

	if (object)
	{
		//Transform
		ImGui::Text("Name: %s		", object->GetName().c_str()); ImGui::SameLine();
		ImGui::Text("Parent: %s", object->GetParent()->GetName().c_str());
		ImGui::Separator();
		ComponentTransform* comp_trans = object->GetComponentTransform();
		if (comp_trans && ImGui::CollapsingHeader("Transform"))
		{
			//Position / Rotation / Scale
			ImGui::InputFloat3("Position", (float*)&comp_trans->local_position, 2);
			ImGui::InputFloat3("Scale", (float*)&comp_trans->local_scale, 2);
			ImGui::InputFloat3("Rotation", (float*)&comp_trans->local_rotation_euler, 2);
			ImGui::Separator();
		}

		//Mesh
		ComponentMesh* comp_mesh = object->GetComponentMesh();
		if (comp_mesh && ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::PushID("Mesh");
			ImGui::Checkbox("Active", &comp_mesh->active);
			ImGui::Checkbox("Normal Vertex", &comp_mesh->normal_show); ImGui::SameLine();
			ImGui::Checkbox("Normal Face", &comp_mesh->normal_face_show);
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
	}
	ImGui::End();
	return UPDATE_CONTINUE;
}
