#include "Panel.h"
#include "PanelHelp.h"
#include "imgui/imgui.h"

update_status PanelHelp::Draw() {
	if (ImGui::BeginMenu(name)) {
		ImGui::EndMenu();
	}
	return UPDATE_CONTINUE;
}

