#include "PanelInspector.h"
#include "imgui/imgui.h"

update_status PanelInspector::Draw()
{
	ImGui::Begin(name);
	ImGui::Text("Transform");
	/*ImGui::InputInt("",)*/


	ImGui::End();

	return UPDATE_CONTINUE;
}
