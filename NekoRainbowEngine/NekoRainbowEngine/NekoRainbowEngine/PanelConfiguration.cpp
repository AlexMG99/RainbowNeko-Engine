#include "Panel.h"
#include "PanelConfiguration.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

PanelConfiguration::~PanelConfiguration()
{
}

update_status PanelConfiguration::Draw()
{
	 update_status ret = UPDATE_CONTINUE;
	 if (ImGui::Begin(name, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoTitleBar)) {
		 ImGui::SetWindowSize(ImVec2(300, 700));
		/* if (ImGui::MenuItem("LOL")){*/

			 if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_Bullet))
			 {
			 }
			 if (ImGui::CollapsingHeader("Window Settings", ImGuiTreeNodeFlags_DefaultOpen))
			 {
			 }
			 if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
			 {
				 int x, y;

				 ImGui::Text("Mouse Position:"); ImGui::TextColored({ 255,216,0,100 }, "X axis : %i, Y axis : %i", App->input->GetMouseX(), App->input->GetMouseY());
				 ImGui::Text("Mouse Motion:  "); ImGui::TextColored({ 255,216,0,100 }, "X axis : %i, Y axis : %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
				 ImGui::Text("Mouse Wheel:   "); ImGui::TextColored({ 255,216,0,100 }, "Z axis : %i", App->input->GetMouseZ());

			 }
			 if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
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
	 return ret;
}
