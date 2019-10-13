#include "PanelTopbar.h"
#include "PanelConsole.h"
#include "PanelHelp.h"
#include "PanelConfiguration.h"
#include "imgui/imgui_docking.h"

#include "Application.h"

bool PanelTopbar::Start()
{
	//Create Panels
	panel_topbar_map.insert(std::pair<std::string, Panel*>("Help", new PanelHelp("Help")));
	panel_map.insert(std::pair<std::string, Panel*>("Configuration", new PanelConfiguration("Configuration")));

	panel_console = new PanelConsole("Console");

	//Start other Panels
	for (auto it_panel_top = panel_topbar_map.begin(); it_panel_top != panel_topbar_map.end(); ++it_panel_top) {
		(*it_panel_top).second->Start();
	}

	for (auto it_panel = panel_map.begin(); it_panel != panel_map.end(); ++it_panel) {
		(*it_panel).second->Start();
	}

	panel_console->Start();

	return false;
}

update_status PanelTopbar::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	//Draw Topbar Panel
	ImGui::BeginMainMenuBar();

	//File Menu
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit"))
			ret = UPDATE_STOP;

		ImGui::EndMenu();
	}


	//Help Menu
	for (auto it_panel_top = panel_topbar_map.begin(); it_panel_top != panel_topbar_map.end(); ++it_panel_top) {
		(*it_panel_top).second->Draw();
	}

	ImGui::EndMainMenuBar();


	ImVec2 display_size = ImGui::GetIO().DisplaySize;
	display_size.x -= 900;
	display_size.y -= 300;
	ImGui::SetNextWindowSize(display_size);
	ImGui::SetNextWindowPos(ImVec2(0, 19));

	ImGui::Begin("PanelEditor", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

	CreateStyle();

	ImGui::BeginDockspace();

	for (auto it_panel = panel_map.begin(); it_panel != panel_map.end(); ++it_panel) {
		(*it_panel).second->Draw();
	}
	ImGui::EndDockspace();
	ImGui::End();

	ImVec2 display_size2 = ImGui::GetIO().DisplaySize;
	display_size2.y -= 765;
	ImGui::SetNextWindowSize(display_size2);
	ImGui::SetNextWindowPos(ImVec2(0, 745));

	ImGui::Begin("PanelC", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);



	ImGui::BeginDockspace();

	panel_console->Draw();



	ImGui::EndDockspace();
	ImGui::End();

	return ret;
}

void PanelTopbar::CreateStyle()
{
	//Set Up Style

	ImGuiStyle& style = ImGui::GetStyle();

	//style.Alpha = 1.0f;
	/*style.WindowFillAlphaDefault = 1.0f;*/
	style.FrameRounding = 4;
	style.IndentSpacing = 12.0f;


	style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.85f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.74f, 0.30f, 0.56f);  
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.95f, 0.63f, 0.44f, 0.47f); 
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.63f, 0.44f, 0.47f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 0.40f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.42f, 0.75f, 0.40f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.42f, 0.75f, 0.40f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.42f, 0.75f, 0.40f, 1.0f); 
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.42f, 0.75f, 0.40f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.42f, 0.75f, 0.40f, 1.0f);
	/*style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);*/
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.89f, 0.98f, 1.00f, 0.99f);
	//style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
	//style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.16f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotHistogram]= ImVec4(0.72f, 1.00f, 1.00f, 0.86f);


}


