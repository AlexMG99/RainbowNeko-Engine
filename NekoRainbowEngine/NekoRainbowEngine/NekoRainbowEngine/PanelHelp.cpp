#include "Panel.h"
#include "PanelHelp.h"

#include "imgui/imgui.h"

update_status PanelHelp::Draw() {
	if (ImGui::BeginMenu("Documentation"))
	{
		if (ImGui::MenuItem("Open Github repository"))
			RequestBrowser("https://github.com/AlexMG99/RainbowNeko-Engine");
		if (ImGui::MenuItem("Download Latest"))
			RequestBrowser("");
		ImGui::EndMenu();
	}
	return UPDATE_CONTINUE;
}

void PanelHelp::RequestBrowser(const char* link)
{
	ShellExecuteA(0, "open", link, 0, 0, 0);
}

