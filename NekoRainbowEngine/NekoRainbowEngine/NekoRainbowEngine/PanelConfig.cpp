#include "PanelConfig.h"
#include "imgui/imgui.h"
#include "ModuleWindow.h"

#include "Application.h"

update_status PanelConfig::Draw() {
	
	if (ImGui::Button(name))
		open = true;

	//Check ConfigWindow
	if (open) 
		ConfigWindow();
	
	return UPDATE_CONTINUE;
}

void PanelConfig::ConfigWindow()
{
	static char project_name[56] = TITLE;
	static char organization_name[56];
	static int fps = 60;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Configuration Window", &open, window_flags))
		if (ImGui::CollapsingHeader("Application"))
		{
			if (ImGui::InputText("Project Name", project_name, IM_ARRAYSIZE(project_name))) 
				App->window->SetTitle(project_name);
			if (ImGui::InputText("Organization", organization_name, IM_ARRAYSIZE(organization_name)))
				App->window->SetOrganization(organization_name);
			if (ImGui::SliderInt("Max FPS", &fps, 1, 120))
				App->CapFPS(fps);

			//Histogram
			static char title[25];

			if (check_time.ReadMs() > 500)
			{
				//Avg FPS
				fps_log[fps_current_log] = current_frames = App->GetAvgFPS();
				fps_current_log++;

				if (fps_current_log >= MAX_HISTOGRAM_LOG)
					fps_current_log = 0;

				//Avg Ms
				ms_log[ms_current_log] = current_ms = App->GetAvgMs();
				ms_current_log++;

				if (ms_current_log >= MAX_HISTOGRAM_LOG)
					ms_current_log = 0;

				PERF_START(check_time);
			}

			sprintf_s(title, 25, "Framerate %.1f", current_frames);
			ImGui::PlotHistogram("##framerate", fps_log, MAX_HISTOGRAM_LOG, fps_current_log, title, 0.0f, 100.0f, ImVec2(310, 100));

			sprintf_s(title, 25, "Milliseconds %.1f", current_ms);
			ImGui::PlotHistogram("##milliseconds", ms_log, MAX_HISTOGRAM_LOG, ms_current_log, title, 0.0f, 40.0f, ImVec2(310, 100));
		}


	    if (ImGui::CollapsingHeader("Window"))
	    {
			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen_on)) {
				App->window->SetFullscreen();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->resizable_on)) {
				App->window->SetResizable();
			}
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
