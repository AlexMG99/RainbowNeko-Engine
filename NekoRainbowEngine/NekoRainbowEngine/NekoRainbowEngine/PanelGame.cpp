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
	size = ImVec2(App->renderer3D->GetTextureSize());

	ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(),size, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	return UPDATE_CONTINUE;
}
