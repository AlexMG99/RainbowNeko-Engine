#include "Application.h"
#include "PanelConfiguration.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler.h"
#include <string>
PanelConfiguration::~PanelConfiguration()
{
}

bool PanelConfiguration::Start()
{
	BROFILER_CATEGORY("Start_PanelConfig", Profiler::Color::LimeGreen);

	JSON_Object* obj = json_object(App->settings_doc);
	JSON_Object* win_obj = json_object_get_object(json_object_get_object(obj, "Application"), "Window");

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
	vsync = json_object_get_boolean(win_obj, "VSYNC");
	App->window->GetVsync();

	
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

	BROFILER_CATEGORY("Draw_PanelConfig", Profiler::Color::GoldenRod);

	update_status ret = UPDATE_CONTINUE;

	ImGui::Begin(name, &enabled);
		AppSettings();

		WindowSettings();

		InputSettings();

		HardwareSettings();

		RendererSettings();

	ImGui::End();


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
	
		if (ImGui::Checkbox("Vsync", &vsync)) {
			App->window->SetVsync(vsync);
		}

	}
}



	void PanelConfiguration::WindowSettings()
	{
		if (ImGui::CollapsingHeader("Window Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brigthness, 0.0f, 2.0f, "%.02f"))
				App->window->SetBrightness();

			if (ImGui::SliderInt("Width", &App->window->width, 800, 1920, "%i"))
				App->window->SetWindowSize();

			if (ImGui::SliderInt("Height", &App->window->height, 600, 1080, "%.1f"))
				App->window->SetWindowSize();

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
		if (ImGui::CollapsingHeader("Input"))
		{
			int x, y;

			ImGui::Text("Mouse Position:"); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "X axis : %i, Y axis : %i", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse Motion:  "); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "X axis : %i, Y axis : %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse Wheel:   "); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f),"Z axis : %i", App->input->GetMouseZ());

			ImGui::Text("ImGui Mouse Position:"); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "X axis : %i, Y axis : %i", (int)App->input->GetImGuiMouse().x, (int)App->input->GetImGuiMouse().y);
		}
	}

	void PanelConfiguration::HardwareSettings()
	{

		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_version compiled;

			ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", &compiled);
			ImGui::Separator();
			ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%i (Cache: %i kb) ", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%.2f GB", (float(SDL_GetSystemRAM()) / 1024.f));
			ImGui::Text("OS:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%s", SDL_GetCurrentVideoDriver());
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

			std::wstring GPU;
			char GPU_Brand[50];
			uint VendorId;
			uint DevideId;
			Uint64 VMB;
			Uint64 VMCU;
			Uint64 VMA;
			Uint64 VMR;
			float conversion = (1024.f * 1024.f);

			/*if (getGraphicsDeviceInfo(NULL, NULL, &GPU, &VMB, &VMCU, &VMA, &VMR))
			{
				VMB = VMB / conversion;
				VMCU = VMCU / conversion;
				VMA = VMA / conversion;
				VMR = VMR / conversion;
			}*/
			
			ImGui::Text("Mode: ");
			ImGui::RadioButton("VRAM Memory", &mode, 1);  ImGui::SameLine();
			ImGui::RadioButton(" RAM Memory", &mode, 2); 

			ImGui::Separator();

			if (mode == 1) 
			{

				ImGui::Text("GPU:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%s", glGetString(GL_VERSION));
				ImGui::Text("Brand:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%s", glGetString(GL_VENDOR));


				/*ImGui::Text("VRAM Budget"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%.1f Mb", (float)VMB);
				ImGui::Text("VRAM Current Usage"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%.1f Mb", (float)VMCU);
				ImGui::Text("VRAM Aviable"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%.1f Mb", (float)VMA);
				ImGui::Text("VRAM Reserved"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%.1f Mb", (float)VMR);*/

				/*ImGui::Text("VRAM Budget: ");
				ImGui::SameLine();
				int temp = 0;
				glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &temp);
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);

				ImGui::Text("VRAM Usage: ");
				ImGui::SameLine();
				glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &temp);
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);

				ImGui::Text("VRAM Available: ");
				ImGui::SameLine();
				glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &temp);
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);

				ImGui::Text("VRAM Reserved: ");
				ImGui::SameLine();
				glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &temp);
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);*/
			}

			if (mode == 2) 
			{
				/*ImGui::Text("Total Reported Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().totalReportedMemory);
				ImGui::Text("Peak Reported Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().peakReportedMemory);
				ImGui::Text("Total Actual Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f),"%d", m_getMemoryStatistics().totalActualMemory);
				ImGui::Text("Peak Actual Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().peakActualMemory);
				ImGui::Text("Total Alloc Unit Count:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().totalAllocUnitCount);
				ImGui::Text("Peak Alloc Unit Count:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().peakAllocUnitCount);
				ImGui::Text("Accumulated Reported Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().accumulatedReportedMemory);
				ImGui::Text("Accumulated Actual Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().accumulatedActualMemory);
				ImGui::Text("Accumulated Alloc Memory:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.91f, 0.22f, 0.27f, 1.00f), "%d", m_getMemoryStatistics().accumulatedActualMemory);*/
			}
			ImGui::Separator();


		}
	}

	void PanelConfiguration::RendererSettings()
	{

		if (ImGui::CollapsingHeader("Renderer"))
		{
			
			static float ambientcolor[3] = { 1.0f, 0.4f, 0.3f };
			static float defaultbackground_color[3] = { 0.0f,0.0f,0.0f};

			if (ImGui::ColorPicker4("Change Render Window Backround Color", ambientcolor))
			{

				glClearColor(ambientcolor[0], ambientcolor[1], ambientcolor[2], 1);
			}
			if (ImGui::Button("Reset Background Color"))
			{
				glClearColor(defaultbackground_color[0], defaultbackground_color[1], defaultbackground_color[2], 1);
			}
			

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


