#include "PanelGame.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Application.h"
#include "Brofiler/Brofiler.h"
#include <string>

update_status PanelGame::Draw()
{
	BROFILER_CATEGORY("Draw_PanelGame", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	size = App->window->GetWinSize();
	ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(),ImVec2(size.x*0.6, size.y*0.6), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	return UPDATE_CONTINUE;
}
