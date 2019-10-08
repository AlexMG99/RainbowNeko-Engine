#include "Panel.h"
#include "PanelDebug.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Application.h"
#include <string>

bool Start()
{
	return true;
}

bool PanelDebug::Start()
{
	return true;
}

update_status PanelDebug::Draw()
{
	if (ImGui::BeginMenu(name)) {
		if (ImGui::MenuItem("Console") && !open)
			open = true;
		if (ImGui::MenuItem("Renderer") && !open)
			open = true;
		if (ImGui::MenuItem("Save"));
		ImGui::EndMenu();
	}

	if (open)
		RendererWindow();

	return UPDATE_CONTINUE;

}

void PanelDebug::RendererWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("Renderer Window", &open, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));
		ImGui::SetWindowPos(ImGui::GetWindowPos(), ImGuiCond_FirstUseEver);

		/*ImGui::Checkbox("GL DEPTH", SetGLProperty(hex_value));*/



		ImGui::End();
	}
}

void PanelDebug::SetGLProperty(int hex_value)
{
	glIsEnabled((GLenum)hex_value) ? glDisable((GLenum)hex_value) : glEnable((GLenum)hex_value);
}
