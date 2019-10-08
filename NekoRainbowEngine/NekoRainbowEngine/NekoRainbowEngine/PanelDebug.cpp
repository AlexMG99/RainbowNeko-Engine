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
		Renderer();

	return UPDATE_CONTINUE;

}

void PanelDebug::Renderer()
{
}
