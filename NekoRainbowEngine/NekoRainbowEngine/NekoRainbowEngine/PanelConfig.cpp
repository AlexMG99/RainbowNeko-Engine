#include "PanelConfig.h"
#include "imgui/imgui.h"
#include "ModuleWindow.h"

#include "Application.h"

update_status PanelConfig::Draw() {
	
	if (ImGui::BeginMenu(name))
	{
		open = true;

		/*ImGui::InputText("Project Name", project_name, IM_ARRAYSIZE(project_name));*/
		ImGui::EndMenu();
	}
	//Check ConfigWindow
	if (open) 
		ConfigWindow();
	
	return UPDATE_CONTINUE;
}

void PanelConfig::ConfigWindow()
{
	static char project_name[56] = TITLE;
	static char organization_name[56];
	static int size = 120;

	

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Configuration Window", &open, window_flags))
		if (ImGui::CollapsingHeader("Configuration"))
		{
			
		} 
	    if (ImGui::CollapsingHeader("Application")) 
	    {
		ImGui::InputText("Project Name", project_name, IM_ARRAYSIZE(project_name));
		ImGui::InputText("Organization", organization_name, IM_ARRAYSIZE(organization_name));
		ImGui::SliderInt("Max FPS", &size, 1, 120);

		/*static char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100)); */
	    }
	    if (ImGui::CollapsingHeader("Window"))
	    {
	    }
		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_version compiled;

			ImGui::Text("SDL Version: %d", &compiled);
			ImGui::Separator();

			ImGui::Text("CPUs: %i (Cache: %i)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM: %i", SDL_GetSystemRAM());
			
			
			ImGui::Text("Caps:");
			ImGui::Separator();
			ImGui::Text("GPU:");
			//SDL_Log("SDL_Version: %d", SDL_GetVersion());
			ImGui::Text("Brand:");
			ImGui::Text("VRAM Budget:");
			ImGui::Text("VRAM Usage:");
			ImGui::Text("VRAM Aviable:");
			ImGui::Text("VRAM Reserved:");
		}
		ImGui::End();

		
	}

