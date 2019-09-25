#include "PanelConfig.h"
#include "imgui/imgui.h"

update_status PanelConfig::Draw() {
	static char project_name[256];
	ImGui::Begin(name);
	ImGui::InputText("Project Name", project_name, IM_ARRAYSIZE(project_name));
	ImGui::End();

	return UPDATE_CONTINUE;
}
