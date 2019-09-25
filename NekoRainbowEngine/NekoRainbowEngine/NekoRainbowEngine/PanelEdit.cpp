#include "Panel.h"
#include "PanelEdit.h"
#include "imgui/imgui.h"

update_status PanelEdit::Draw() {
	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {};
		if (ImGui::MenuItem("Redo", "CTRL+Y")) {};
		ImGui::Separator();

		if (ImGui::MenuItem("Copy", "CTRL+C")) {};
		if (ImGui::MenuItem("Cut", "CTRL+X")) {};
		if (ImGui::MenuItem("Paste", "CTRL+V")) {};
		ImGui::Separator();

		ImGui::EndMenu();
	}

	return UPDATE_CONTINUE;
}