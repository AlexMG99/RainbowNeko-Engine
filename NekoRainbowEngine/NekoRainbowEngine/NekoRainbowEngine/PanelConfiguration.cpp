#include "Panel.h"
#include "PanelConfiguration.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_docking.h"

#include <string>
PanelConfiguration::~PanelConfiguration()
{
}

bool PanelConfiguration::Start()
{
	JSON_Object* obj = json_object(App->scene_test->settings_doc);
	JSON_Object* app_obj = json_object_get_object(obj, "Application");

	//Set window attributes
	strcpy_s(App->window->project_name, json_object_get_string(app_obj, "Title"));
	App->window->SetWindowSize(json_object_get_number(json_object_get_object(obj, "Application"), "Width"),
		json_object_get_number(json_object_get_object(obj, "Application"), "Height"));
	App->window->SetTitle(App->window->project_name);
	App->window->resizable_on = json_object_get_boolean(json_object_get_object(obj, "Application"), "Resizable");
	App->window->SetResizable();
	App->window->fullscreen_on = json_object_get_boolean(json_object_get_object(obj, "Application"), "Fullscreen");
	App->window->SetFullscreen();
	App->window->fullscreendesktop_on = json_object_get_boolean(json_object_get_object(obj, "Application"), "Fullscreen Desktop");
	App->window->SetFullscreenDesktop();
	App->window->border_on = json_object_get_boolean(json_object_get_object(obj, "Application"), "Border");
	App->window->SetBorderless();
	App->window->SetBrightness(json_object_get_number(json_object_get_object(obj, "Application"), "Brightness"));

	App->CapFPS(capped_fps);


	//Renderer attributes
	gl_depth = glIsEnabled(GL_DEPTH);
	gl_cull_face = glIsEnabled(GL_CULL_FACE);
	gl_lighting = glIsEnabled(GL_LIGHTING);
	gl_color_material = glIsEnabled(GL_COLOR_MATERIAL);
	gl_texture_2d = glIsEnabled(GL_TEXTURE_2D);
	gl_fog = glIsEnabled(GL_FOG);
	gl_shade_model = glIsEnabled(GL_SHADE_MODEL);


	return true;
}

update_status PanelConfiguration::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::BeginDock("Configuration", false, &visible, true);
	
		AppSettings();

		WindowSettings();

		InputSettings();

		HardwareSettings();

		RendererSettings();
		
		
	ImGui::EndDock();
	
	return ret;
}


void PanelConfiguration::AppSettings()
{
	static char organization_name[56];

	if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::InputText("Project Name", App->window->project_name, IM_ARRAYSIZE(App->window->project_name)))
			App->window->SetTitle(App->window->project_name);
		if (ImGui::InputText("Organization", organization_name, IM_ARRAYSIZE(organization_name)))
			App->window->SetOrganization(organization_name);
		if (ImGui::SliderInt("Max FPS", &capped_fps, 10, 150))
			App->CapFPS(capped_fps);

		ImGui::Text("Limit Framerate: "); ImGui::SameLine();
		ImGui::TextColored({ ImVec4(0.91f, 0.22f, 0.27f, 1.00f) }, "%i", App->GetCurrFPS());

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
}



	void PanelConfiguration::WindowSettings()
	{
		if (ImGui::CollapsingHeader("Window Settings", ImGuiTreeNodeFlags_DefaultOpen))
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
	}

	void PanelConfiguration::InputSettings()
	{
		if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
		{
			int x, y;

			ImGui::Text("Mouse Position:"); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "X axis : %i, Y axis : %i", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse Motion:  "); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "X axis : %i, Y axis : %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse Wheel:   "); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f),"Z axis : %i", App->input->GetMouseZ());

		}
	}

	void PanelConfiguration::HardwareSettings()
	{

		if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
		{

			SDL_version compiled;

			ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", &compiled);
			ImGui::Separator();
			ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%i (Cache: % i)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%i Gb", SDL_GetSystemRAM() / 1024);

			static std::string caps = "";
			caps += SDL_HasRDTSC() ? "RDTSC, " : "";
			caps += SDL_HasMMX() ? "MMX, " : "";
			caps += SDL_HasAVX() ? "AVX, " : "";
			caps += SDL_HasSSE() ? "SEE, " : "";
			caps += SDL_HasSSE2() ? "SEE2, " : "";
			caps += SDL_HasSSE3() ? "SEE3, " : "";
			caps += SDL_HasSSE41() ? "SEE41, " : "";
			caps += SDL_HasSSE42() ? "SEE42 " : "";

			ImGui::Text("Caps:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), caps.c_str());
			caps = "";

			ImGui::Separator();

			GLint totalmemory = 0;
			GLint currentmemoryaviable = 0;
			GLint info = 0;

			ImGui::Text("GPU:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f) , "%s", glGetString(GL_VERSION));
			ImGui::Text("Brand:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%s", glGetString(GL_VENDOR));

			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalmemory);
			ImGui::Text("VRAM Usage:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%i Mb", totalmemory / 1024);

			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentmemoryaviable);
			ImGui::Text("VRAM Aviable:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%i Mb", currentmemoryaviable / 1024);

			ImGui::Text("VRAM Reserved:");


		}
	}

	void PanelConfiguration::RendererSettings()
	{

		if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
		{

			if (gl_depth) glEnable(GL_DEPTH); else glDisable(GL_DEPTH);
			ImGui::Checkbox("GL DEPTH", &gl_depth); ImGui::SameLine(150);

			if (gl_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
			ImGui::Checkbox("GL CULL FACE", &gl_cull_face);

			if (gl_lighting) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
			ImGui::Checkbox("GL LIGHTING", &gl_lighting); ImGui::SameLine(150);

			if (gl_color_material) glEnable(GL_COLOR_MATERIAL); else glDisable(GL_COLOR_MATERIAL);
			ImGui::Checkbox("GL COLOR MATERIAL", &gl_color_material);

			if (gl_texture_2d) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
			ImGui::Checkbox("GL TEXTURE 2D", &gl_texture_2d); ImGui::SameLine(150);

			if (gl_fog) glEnable(GL_FOG); else glDisable(GL_FOG);
			ImGui::Checkbox("GL FOG", &gl_fog);

			if (gl_shade_model) glEnable(GL_SHADE_MODEL); else glDisable(GL_SHADE_MODEL);
			ImGui::Checkbox("GL SHADE MODEL", &gl_shade_model);

			ImGui::Separator();

			ImGui::Checkbox("GL FILL", &gl_fill); ImGui::SameLine(110);

			ImGui::Checkbox("GL WIREFRAME", &gl_lines); ImGui::SameLine(250);

			ImGui::Checkbox("GL VERTEX", &gl_points); 

			if (gl_fill) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			if (gl_lines) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			if (gl_points) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			}

			
		}

	}


