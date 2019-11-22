#include "Application.h"
#include "GL/include/glew.h"
#include "PanelInspector.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentCamera.h"
#include "imgui/imgui.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Brofiler/Brofiler.h"
#include <list>

update_status PanelInspector::Draw()
{

	BROFILER_CATEGORY("Draw_PaneInspector", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_HorizontalScrollbar);

	GameObject* object = App->viewport->selected_object;

	if (object)
	{
		//Components
		ComponentTransform* comp_trans = object->GetComponentTransform();
		ComponentMesh* comp_mesh = object->GetComponentMesh();
		ComponentTexture* comp_texture = object->GetComponentTexture();
		ComponentCamera* comp_camera = object->GetComponentCamera();

		//Transform
		ImGui::PushID("Transform");
		ImGui::Checkbox("", &object->active); ImGui::SameLine();
		ImGui::Text("%s		", object->GetName().c_str()); ImGui::SameLine();
		ImGui::Checkbox("Static", &object->is_static);
		ImGui::Separator();

		if (comp_trans && ImGui::CollapsingHeader("Transform"))
		{
			//Position / Rotation / Scale
			if (((comp_camera) ? (ImGui::InputFloat3("Position", (float*)&comp_camera->frustum.pos, 2)) : (ImGui::InputFloat3("Position", (float*)&comp_trans->local_position, 2))) ||
				ImGui::InputFloat3("Rotation", (float*)&comp_trans->local_rotation_euler, 2)||
				ImGui::InputFloat3("Scale", (float*)&comp_trans->local_scale, 2))
			{
				comp_trans->UpdateComponents();
			}

			ImGui::Separator();
		}

		ImGui::PopID();
		//Mesh
		if (comp_mesh && ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::PushID("Mesh");
			ImGui::Checkbox("Active", &comp_mesh->active);
			ImGui::Checkbox("Normal Vertex", &comp_mesh->normal_show); ImGui::SameLine();
			ImGui::Checkbox("Normal Face", &comp_mesh->normal_face_show);
			ImGui::Text("Resource: %u", comp_mesh->mesh->GetID().GetNumber());
			ImGui::Text("Id vertices: %i", comp_mesh->mesh->buffers[BUFF_VERTICES]);
			ImGui::Text("Num vertices: %i", comp_mesh->mesh->vertices_size);
			ImGui::Text("Id indices: %i", comp_mesh->mesh->buffers[BUFF_INDEX]);
			ImGui::Text("Num indices: %i", comp_mesh->mesh->index_size);
			ImGui::Text("Id uv: %i", comp_mesh->mesh->buffers[BUFF_UV]);
			ImGui::Separator();
		
			ImGui::Text("Bounding Boxes:");
			ImGui::Checkbox("Show AABB", &comp_mesh->BB_mesh.show_aabb);
			ImGui::Checkbox("Show OBB", &comp_mesh->BB_mesh.show_obb);
			ImGui::Separator();

			ImGui::Text("Mode: ");
			ImGui::RadioButton("Wireframe", &mode, 1); ImGui::SameLine();
			ImGui::RadioButton("Vertex", &mode, 2);
			if (mode == 1) 
			{
				ImGui::ColorPicker4("Change Wireframe Color", &comp_mesh->wireframe_color);
				ImGui::SliderInt("Line Width", &comp_mesh->line_width, 1, 10, "%i");
			}
			if (mode == 2) 
			{
				ImGui::ColorPicker4("Change Vertex Color", &comp_mesh->vertex_color);
				ImGui::SliderInt("Point Size", &comp_mesh->point_size, 1, 10, "%i");
			}
			
			ImGui::PopID();
			ImGui::Separator();
		}
		
		//Texture
		if (comp_texture)
		{
			if (ImGui::CollapsingHeader("Texture")) 
			{
				ImGui::PushID("Texture");
				ImGui::Checkbox("Active", &comp_texture->active);
				ImGui::Text("Resource: %u", comp_texture->texture->GetID().GetNumber());
				ImGui::Text("Path: %s", comp_texture->texture->imported_file.c_str());
				ImGui::Text("Id texture: %i", comp_texture->texture->image_id);
				ImGui::Text("W: %i		H: %i", comp_texture->texture->width, comp_texture->texture->height);
				ImGui::Image((ImTextureID)comp_texture->texture->image_id, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::PopID();
				ImGui::Separator();
			}
		}

		if (comp_camera)
		{
			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::PushID("Camera");
				ImGui::Checkbox("Show Frustrum", &comp_camera->draw_frustrum); ImGui::Separator();

				if (ImGui::InputFloat("Near Plane", &comp_camera->frustum.nearPlaneDistance, 5) ||
					ImGui::InputFloat("Far Plane", &comp_camera->frustum.farPlaneDistance, 5) ||
					ImGui::InputFloat("FOV", &comp_camera->frustum.verticalFov, 0.1))
				{
					comp_camera->UpdateFrustum(false);
					comp_camera->update_proj = true;
				}
				ImGui::PopID();
				ImGui::Separator();
			}
		}
	}

	ImGui::End();
	return UPDATE_CONTINUE;
}
