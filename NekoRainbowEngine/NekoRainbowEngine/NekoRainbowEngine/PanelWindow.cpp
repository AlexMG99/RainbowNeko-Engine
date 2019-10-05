#include "Panel.h"
#include "PanelWindow.h"
#include "imgui/imgui.h"

update_status PanelWindow::Draw() {
	if (ImGui::BeginMenu(name)) {
		if (ImGui::Begin("Configuration")) {

			ImGui::End();
		}

		ImGui::EndMenu();
	}

	return UPDATE_CONTINUE;
}