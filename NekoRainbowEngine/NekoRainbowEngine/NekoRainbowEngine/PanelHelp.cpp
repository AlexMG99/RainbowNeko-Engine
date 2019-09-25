#include "Panel.h"
#include "PanelHelp.h"

#include "imgui/imgui.h"

static bool open = false;

update_status PanelHelp::Draw() {
	//Documentation Links & About
	if (ImGui::BeginMenu(name))
	{
		if (ImGui::MenuItem("Github repository\t"))
			RequestBrowser("https://github.com/AlexMG99/RainbowNeko-Engine");
		if (ImGui::MenuItem("Documentation\t"))
			RequestBrowser("https://github.com/AlexMG99/RainbowNeko-Engine/wiki");
		if (ImGui::MenuItem("Download latest\t"))
			RequestBrowser("https://github.com/AlexMG99/RainbowNeko-Engine/releases");
		if (ImGui::MenuItem("Report a bug\t"))
			RequestBrowser("https://github.com/AlexMG99/RainbowNeko-Engine/issues");
		if (ImGui::MenuItem("About\t") && !open)
			open = true;
		ImGui::EndMenu();
	}

	ImGui::ShowDemoWindow();

	//Checks About
	if(open)
		AboutWindow();

	return UPDATE_CONTINUE;
}

void PanelHelp::RequestBrowser(const char* link)
{
	ShellExecuteA(0, "open", link, 0, 0, 0);
}

void PanelHelp::AboutWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("About", &open, window_flags)) {
		ImGui::End();
	}
}

