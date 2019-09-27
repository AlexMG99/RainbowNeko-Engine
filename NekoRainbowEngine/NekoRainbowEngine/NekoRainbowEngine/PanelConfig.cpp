#include "PanelConfig.h"
#include "imgui/imgui.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Application.h"

bool PanelConfig::Start()
{
	JSON_Object* obj = json_object(App->scene_test->settings_doc);
	project_name = (char*)json_object_get_string(json_object_get_object(obj, "Application"), "Title");

	return true;
}

update_status PanelConfig::Draw() {
	
	if (ImGui::Button(name))
		open = true;

	//Check ConfigWindow
	if (open) 
		ConfigWindow();
	
	return UPDATE_CONTINUE;
}

bool PanelConfig::Save()
{

	return true;
}

void PanelConfig::ConfigWindow()
{
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
			if (ImGui::SliderFloat("Brightness", &App->window->brigthness, 0.0f, 1.0f, "%.1f"))
				App->window->SetBrightness();

			if (ImGui::SliderInt("Width", &App->window->width, 0, 1920, "%i"))
				App->window->SetWindowSize();

			if (ImGui::SliderInt("Height", &App->window->height, 0, 1080, "%.1f"))
				App->window->SetWindowSize();

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen_on)) {
				App->window->SetFullscreen();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->resizable_on)) {
				App->window->SetResizable();
			}

			if (ImGui::Checkbox("Borderless", &App->window->borderless_on)) {
				App->window->SetBorderless();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &App->window->fullscreendesktop_on)) {
				App->window->SetFullscreenDesktop();
			}

	    }
		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_version compiled;



			ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%d", &compiled);
			ImGui::Separator();
			ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i (Cache: % i)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Gb", SDL_GetSystemRAM()/1024);
			
			static std::string caps = "";
			caps += SDL_HasRDTSC() ? "RDTSC, " : "";

			caps += SDL_HasMMX() ? "MMX, " : "";

			caps += SDL_HasAVX() ? "AVX, " : "";

			caps += SDL_HasSSE() ? "SEE, " : "";

			caps += SDL_HasSSE2() ? "SEE2, " : "";

			caps += SDL_HasSSE3() ? "SEE3, " : "";

			caps += SDL_HasSSE41() ? "SEE41, " : "";

			caps += SDL_HasSSE42() ? "SEE42 " : "";

			ImGui::Text("Caps:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, caps.c_str());
			caps = "";

			ImGui::Separator();

			GLint totalmemory = 0;
			GLint currentmemoryaviable = 0;
			GLint info = 0;

			ImGui::Text("GPU:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", glGetString(GL_VERSION));
			ImGui::Text("Brand:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", glGetString(GL_VENDOR));
			ImGui::Text("VRAM Budget:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", glGetString(GL_RENDERER));
			
			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalmemory);
			ImGui::Text("VRAM Usage:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", totalmemory/1024 );
			
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentmemoryaviable);
			ImGui::Text("VRAM Aviable:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", currentmemoryaviable/1024);
			

			/*lGetIntegerv(GL_NVX_gpu_memory_info, &info);
			ImGui::Text("VRAM Aviable:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", &info);*/


			ImGui::Text("VRAM Reserved:");
		}
		ImGui::End();

		
	}

