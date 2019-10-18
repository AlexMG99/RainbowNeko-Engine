#include "PanelTopbar.h"
#include "PanelHelp.h"

#include "imgui/imgui_docking.h"

#include "Application.h"

bool PanelTopbar::Start()
{
	//Initialize Panels
	panel_help = new PanelHelp("Help");
	panel_help->Start();

	return true;
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

	panel_help->Draw();

	ImGui::EndMainMenuBar();
    CreateStyle();
	
	return ret;
}

//void PanelTopbar::CreateDocking()
//{
//	ImVec2 size = ImGui::GetIO().DisplaySize;
//	size.y = 975;
//	ImGui::SetNextWindowSize(size);
//	ImGui::SetNextWindowPos(ImVec2(0,19));
//
//	ImGui::Begin("PanelDocking", NULL, ImVec2(0, 19), 1.0f, /*ImGuiWindowFlags_NoMove |*/
//			ImGuiWindowFlags_NoBringToFrontOnFocus | /*ImGuiWindowFlags_NoResize |*/
//			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar /*| ImGuiWindowFlags_NoMouseInputs*/ );
//
//	ImGui::BeginDockspace();
//
//		CreateDockTop();
//		
//	ImGui::EndDockspace();
//	ImGui::End();
//
//}



//void PanelTopbar::CreateDockHier()
//{
//	ImVec2 display_size4 = ImGui::GetIO().DisplaySize;
//	display_size4.x = 150;
//	display_size4.y -= 300;
//	ImGui::SetNextWindowSize(display_size4);
//	ImGui::SetNextWindowPos(ImVec2(280, 19));
//
//	ImGui::Begin("PanelHier", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove |
//		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
//		/*ImGuiWindowFlags_NoScrollbar |*/ ImGuiWindowFlags_NoTitleBar /*ImGuiWindowFlags_NoMouseInputs */);
//
//
//
//	ImGui::BeginDockspace();
//
//	panel_hierarchy->Draw();
//
//	ImGui::EndDockspace();
//
//	ImGui::End();
//}

void PanelTopbar::CreateDockHier()
{
	ImVec2 display_size2 = ImGui::GetIO().DisplaySize;
	
	display_size2.y -= 770;
	ImGui::SetNextWindowSize(display_size2);
	ImGui::SetNextWindowPos(ImVec2(0, 745));

	ImGui::Begin("PanelBottom", NULL, ImVec2(0, 0), 1.0f, 
		/*ImGuiWindowFlags_NoBringToFrontOnFocus |*/ ImGuiWindowFlags_NoResize |/*ImGuiWindowFlags_NoScrollbar |*/ ImGuiWindowFlags_NoTitleBar );
	ImGui::BeginDockspace();

	//panel_console->Draw();

	ImGui::EndDockspace();

	ImGui::End();
}

void PanelTopbar::CreateStyle()
{
	//Set Up Style

	ImGuiStyle& style = ImGui::GetStyle();

	//style.Alpha = 1.0f;
	/*style.WindowFillAlphaDefault = 1.0f;*/
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
	style.Colors[ImGuiCol_Header] = ImVec4(0.655f, 0.198f, 0.501f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.16f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.91f, 0.22f, 0.27f, 1.00f);

}


