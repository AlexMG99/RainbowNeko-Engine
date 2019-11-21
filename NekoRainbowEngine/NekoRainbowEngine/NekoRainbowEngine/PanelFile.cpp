#include "Panel.h"
#include "PanelFile.h"
#include "imgui/imgui.h"



update_status PanelFile::Draw() {

	update_status ret = UPDATE_CONTINUE;
	if (ImGui::BeginMenu(name))
	{
		if (ImGui::MenuItem("Exit"))
			ret = UPDATE_STOP;

		ImGui::EndMenu();
	}
	return ret;
}