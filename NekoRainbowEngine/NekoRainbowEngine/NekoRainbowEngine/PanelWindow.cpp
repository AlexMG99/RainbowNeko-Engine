#include "Application.h"
#include "Panel.h"
#include "PanelWindow.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Parson/parson.h"

#include <string>

bool PanelWindow::Start()
{
	JSON_Object* obj = json_object(App->settings_doc);
	JSON_Object* win_obj = json_object_get_object(json_object_get_object(obj, "Application"),"Window");

	//Set window attributes
	strcpy_s(App->window->project_name, json_object_get_string(win_obj, "Title"));
	App->window->SetWindowSize(json_object_get_number(win_obj, "Width"),
		json_object_get_number(win_obj, "Height"));
	App->window->SetTitle(App->window->project_name);
	App->window->resizable_on = json_object_get_boolean(win_obj, "Resizable");
	App->window->SetResizable();
	App->window->fullscreen_on = json_object_get_boolean(win_obj, "Fullscreen");
	App->window->SetFullscreen();
	App->window->fullscreendesktop_on = json_object_get_boolean(win_obj, "Fullscreen Desktop");
	App->window->SetFullscreenDesktop();
	App->window->border_on = json_object_get_boolean(win_obj, "Border");
	App->window->SetBorderless();
	App->window->SetBrightness(json_object_get_number(win_obj, "Brightness"));

	App->CapFPS(capped_fps);

	return true;
}

update_status PanelWindow::Draw() {
	if (ImGui::BeginMenu(name)) {
		if (ImGui::MenuItem("Configuration") && !open) 
			open = true;
		ImGui::EndMenu();
	}

	if (open)
		ConfigWindow();

	return UPDATE_CONTINUE;
}

update_status PanelWindow::Save()
{
	JSON_Object* object = json_object(App->settings_doc);
	JSON_Object* win_object = json_object_dotget_object(object, "Window");

	json_object_dotset_string(win_object, "Window.Title", App->window->GetTitle());
	json_object_dotset_number(win_object, "Window.Width", App->window->GetWinSize().x);
	json_object_dotset_number(win_object, "Window.Height", App->window->GetWinSize().y);
	json_object_dotset_boolean(win_object, "Window.Resizable", App->window->resizable_on);
	json_object_dotset_boolean(win_object, "Window.Fullscreen", App->window->fullscreen_on);
	json_object_dotset_boolean(win_object, "Window.Fullscreen Desktop", App->window->fullscreendesktop_on);
	json_object_dotset_boolean(win_object, "Window.Border", App->window->border_on);
	json_object_dotset_number(win_object, "Window.Brightness", App->window->brigthness);

	json_serialize_to_file(App->settings_doc, "Settings/win_config.json");

	return UPDATE_CONTINUE;
}

void PanelWindow::ConfigWindow()
{
	static char organization_name[56];

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Configuration Window", &open, window_flags))
		if (ImGui::CollapsingHeader("Application"))
		{
			if (ImGui::InputText("Project Name", App->window->project_name, IM_ARRAYSIZE(App->window->project_name)))
				App->window->SetTitle(App->window->project_name);
			if (ImGui::InputText("Organization", organization_name, IM_ARRAYSIZE(organization_name)))
				App->window->SetOrganization(organization_name);
			if (ImGui::SliderInt("Max FPS", &capped_fps, 10, 150))
				App->CapFPS(capped_fps);

			ImGui::Text("Limit Framerate: "); ImGui::SameLine();
			ImGui::TextColored({ 255,216,0,100 }, "%i", App->GetCurrFPS());

			//Histogram
			static char title[25];

			if (check_time.ReadMs() > 100)
			{
				//Avg FPS
				fps_log[fps_current_log] = current_frames = App->GetCurrFPS();
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
			ImGui::PlotHistogram("##framerate", fps_log, MAX_HISTOGRAM_LOG, fps_current_log, title, 0.0f, 160.0f, ImVec2(310, 100));

			sprintf_s(title, 25, "Milliseconds %.1f", current_ms);
			ImGui::PlotHistogram("##milliseconds", ms_log, MAX_HISTOGRAM_LOG, ms_current_log, title, 0.0f, 40.0f, ImVec2(310, 100));
		}


	if (ImGui::CollapsingHeader("Window Settings"))
	{
		if (ImGui::SliderFloat("Brightness", &App->window->brigthness, 0.0f, 2.0f, "%.02f"))
			App->window->SetBrightness();

		if (ImGui::SliderInt("Width", &App->window->width, 800, 1920, "%i"))
			App->window->SetWindowSize(); ImGui::SameLine();

		if (ImGui::InputInt("W", &App->window->width)) {
			if (App->window->width >= 1920) {
				App->window->width = 1920;
				App->window->SetWindowSize();
			}
			if (App->window->width < 800) {
				App->window->width = 800;
				App->window->SetWindowSize();
			}
		}

		if (ImGui::SliderInt("Height", &App->window->height, 600, 1080, "%.1f"))
			App->window->SetWindowSize(); ImGui::SameLine();

		if (ImGui::InputInt("H", &App->window->height)) {
			if (App->window->height > 1080)
			{
				App->window->height = 1080;
				App->window->SetWindowSize();
			}
			if (App->window->height < 600)
			{
				App->window->height = 600;
				App->window->SetWindowSize();
			}
		}

		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen_on))
			App->window->SetFullscreen(); ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &App->window->resizable_on))
			App->window->SetResizable();

		if (ImGui::Checkbox("Border", &App->window->border_on))
			App->window->SetBorderless(); ImGui::SameLine();

		if (ImGui::Checkbox("Full Desktop", &App->window->fullscreendesktop_on))
			App->window->SetFullscreenDesktop();

	}
	if (ImGui::CollapsingHeader("Input"))
	{
		int x, y;
		
		ImGui::Text("Mouse Position:"); ImGui::TextColored({ 255,216,0,100 }, "X axis : %i, Y axis : %i", App->input->GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse Motion:  "); ImGui::TextColored({ 255,216,0,100 }, "X axis : %i, Y axis : %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
		ImGui::Text("Mouse Wheel:   "); ImGui::TextColored({ 255,216,0,100 }, "Z axis : %i", App->input->GetMouseZ());
	
	
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version compiled;

		ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%d", &compiled);
		ImGui::Separator();
		ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i (Cache: % i)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Gb", SDL_GetSystemRAM() / 1024);

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

		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalmemory);
		ImGui::Text("VRAM Usage:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", totalmemory / 1024);

		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentmemoryaviable);
		ImGui::Text("VRAM Aviable:"); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", currentmemoryaviable / 1024);

		ImGui::Text("VRAM Reserved:");
	}


	ImGui::End();

}
