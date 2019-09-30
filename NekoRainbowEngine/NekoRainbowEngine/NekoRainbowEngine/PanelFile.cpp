#include "Panel.h"
#include "PanelFile.h"
#include "imgui/imgui.h"



update_status PanelFile::Draw() {

	update_status ret = UPDATE_CONTINUE;
	if (ImGui::BeginMenu(name))
	{
		if (ImGui::MenuItem("New Scene", "Ctrl+N")) {};
		if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {};
		if (ImGui::BeginMenu("Open Recent...")) {
			ImGui::MenuItem("Scene1");
			ImGui::EndMenu();
		}
		ImGui::Separator();

		if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {};
		if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {};
		ImGui::Separator();

		if (ImGui::MenuItem("New Project")) {};
		if (ImGui::MenuItem("Open Project")) {};
		if (ImGui::MenuItem("Save Project")) {};
		ImGui::Separator();

		if (ImGui::MenuItem("Build Settings...", "Ctrl+Shift+B")) {};
		if (ImGui::MenuItem("Build And Run")) {};
		ImGui::Separator();

		if (ImGui::MenuItem("Exit")) {
			ret = UPDATE_STOP;
		}

		ImGui::EndMenu();
	}
	return ret;
}