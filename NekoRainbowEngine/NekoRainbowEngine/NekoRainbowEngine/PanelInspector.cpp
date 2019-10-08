#include "PanelInspector.h"

#include "Application.h"

#include <list>

update_status PanelInspector::Draw()
{
	if (ImGui::Begin(name)) {
		for (auto it_cube = App->importer->cube_list.begin(); it_cube != App->importer->cube_list.end(); ++it_cube)
		{
			ImGui::Text("Translate: X:"); ImGui::SameLine();
			static int x = 0, y = 0, z = 0;
			if (ImGui::InputInt("X:", &x, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)
				|| ImGui::InputInt("Y:", &y, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)
				|| ImGui::InputInt("Z:", &z, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {

				par_shapes_set_position((*it_cube)->cube_mesh, x, y, z);
				(*it_cube)->GenerateMesh();
			}
		}
		ImGui::End();
	}

	return UPDATE_CONTINUE;
}
