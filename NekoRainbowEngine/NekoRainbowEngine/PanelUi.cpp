#include "Application.h"
#include "PanelUi.h"

#include <string>

#include "Brofiler/Brofiler.h"


bool PanelUi::Start()
{
	BROFILER_CATEGORY("Start_PanelUi", Profiler::Color::LimeGreen);

	return true;
}

update_status PanelUi::Draw() {
	BROFILER_CATEGORY("Draw_PanelUi", Profiler::Color::GoldenRod);


	if (ImGui::BeginMenu(name))
	{
		if (ImGui::MenuItem("Create a Button"));
		if (ImGui::MenuItem("Create a Label"));


		ImGui::EndMenu();
	}

	if (create_button)
		CreateButton();

	return UPDATE_CONTINUE;
}


void PanelUi::CreateButton()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin("Create a Button", &create_button, window_flags)) {
		ImGui::SetWindowSize(ImVec2(575, 600));
		ImGui::End();
	}
}