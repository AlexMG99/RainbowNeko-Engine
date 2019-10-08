#include "PanelInspector.h"

#include "Application.h"

#include <list>

update_status PanelInspector::Draw()
{
	uint i = 0;
	if (ImGui::Begin(name)) {
		for (auto it_cube = App->importer->cube_list.begin(); it_cube != App->importer->cube_list.end(); ++it_cube)
		{
			std::string id_object = "Gameobject: " + std::to_string(i);
			ImGui::PushID(i);
			ImGui::TextColored({ 0,23,93,255 }, id_object.c_str()); ImGui::Separator();

			//Tranform Position
			ImGui::TextColored({ 226,01,0,255 },"Position: ");
			static float x = (*it_cube)->cube_mesh->points[0], 
				y = (*it_cube)->cube_mesh->points[1], 
				z = (*it_cube)->cube_mesh->points[2];

			ImGui::PushItemWidth(60);
			if (ImGui::InputFloat("X", &(*it_cube)->cube_mesh->points[0], 0.0F, 0.0F, 2)
				|| ImGui::InputFloat("Y", &(*it_cube)->cube_mesh->points[1], 0.0F, 0.0F, 2)
				|| ImGui::InputFloat("Z", &(*it_cube)->cube_mesh->points[2], 0.0F, 0.0F, 2)) {

				float distance_x = (*it_cube)->cube_mesh->points[0] - x,
					distance_y = (*it_cube)->cube_mesh->points[1] - y,
					distance_z = (*it_cube)->cube_mesh->points[2] - z;

				x = (*it_cube)->cube_mesh->points[0];
				y = (*it_cube)->cube_mesh->points[1];
				z = (*it_cube)->cube_mesh->points[2];

				par_shapes_translate((*it_cube)->cube_mesh, distance_x, distance_y, distance_z);
				(*it_cube)->GenerateMesh();
			}

			//// Scale Object
			//ImGui::TextColored({ 0,222,0,255 }, "Scale: ");
			//static float x_scale = 1,
			//	y_scale = 1,
			//	z_scale = 1;

			//if (ImGui::InputFloat("X Scale", &x_scale, 0.0F, 0.0F, 2)
			//	|| ImGui::InputFloat("Y Scale", &y_scale, 0.0F, 0.0F, 2)
			//	|| ImGui::InputFloat("Z Scale", &z_scale, 0.0F, 0.0F, 2)) {

			//	par_shapes_scale((*it_cube)->cube_mesh, x_scale, y_scale, z_scale);
			//	(*it_cube)->GenerateMesh();
			//}

			i++;
			ImGui::PopID();
		}
		ImGui::End();
	}

	return UPDATE_CONTINUE;
}
