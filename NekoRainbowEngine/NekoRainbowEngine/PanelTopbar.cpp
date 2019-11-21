#include "Application.h"
#include "PanelTopbar.h"
#include "PanelScene.h"
#include "PanelHelp.h"
#include "PanelConsole.h"
#include "PanelPlay.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"
#include "PanelConfiguration.h"
#include "PanelShape.h"
#include "PanelImporter.h"
#include "PanelSceneManager.h"
#include "Application.h"
#include "Brofiler/Brofiler.h"

PanelTopbar::~PanelTopbar()
{
	RELEASE(panel_help);
}

bool PanelTopbar::Start()
{
	BROFILER_CATEGORY("Start_PanelTopBar", Profiler::Color::LimeGreen);
	//Initialize Panels
	panel_help = new PanelHelp("Help");
	panel_help->Start();

	return true;
}

update_status PanelTopbar::Draw()
{

	BROFILER_CATEGORY("Draw_PanelTopBar", Profiler::Color::GoldenRod);

	update_status ret = UPDATE_CONTINUE;

	//Draw Topbar Panel
	ImGui::BeginMainMenuBar();

	//File Menu
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit", "Esc"))
			ret = UPDATE_STOP;

		ImGui::EndMenu();
	}

	//Help Menu
	panel_help->Draw();

	//Window Menu
	if (ImGui::BeginMenu("Window"))
	{
		ImGui::MenuItem("Console	", NULL, &App->editor->panel_console->enabled);
		ImGui::MenuItem("Configuration	", NULL, &App->editor->panel_config->enabled);
		ImGui::MenuItem("Game	", NULL, &App->editor->panel_scene->enabled);
		ImGui::MenuItem("Hierarchy	", NULL, &App->editor->panel_hierarchy->enabled);
		ImGui::MenuItem("Inspector	", NULL, &App->editor->panel_inspector->enabled);
		ImGui::MenuItem("SceneManager	", NULL, &App->editor->panel_scene_manager->enabled);
		//ImGui::MenuItem("Panel Shape ", NULL, &App->editor->panel_shape->enabled);
		ImGui::MenuItem("Assets", NULL, &App->editor->panel_importer->enabled);
		ImGui::MenuItem("Play", NULL, &App->editor->panel_play->enabled);

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
    CreateStyle();
	
	return ret;
}

void PanelTopbar::CreateStyle()
{

	BROFILER_CATEGORY("Set_Style", Profiler::Color::MediumPurple);


	//Set Up Style

	ImGuiStyle& style = ImGui::GetStyle();

	style.FrameRounding = 4;
	style.IndentSpacing = 12.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(255, 235, 238, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11, 0.11, 0.11, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.455f, 0.398f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.455f, 0.198f, 0.301f, 0.76f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.455f, 0.198f, 0.301f, 0.76f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.91f, 0.22f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.455f, 0.398f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.455f, 0.398f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.455f, 0.398f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.22f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.91f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.91f, 0.22f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.455f, 0.198f, 0.301f, 0.76f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.655f, 0.198f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.655f, 0.198f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.655f, 0.198f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.91f, 0.22f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.655f, 0.198f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
	/*style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.16f, 0.00f, 1.00f);*/
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
	/*style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);*/
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.91f, 0.22f, 0.27f, 1.00f);

}


