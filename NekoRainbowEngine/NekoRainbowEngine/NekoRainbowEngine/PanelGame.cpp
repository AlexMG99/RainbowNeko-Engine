#include "PanelGame.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_docking.h"
#include "Application.h"
#include <string>

update_status PanelGame::Draw()
{
	ImGui::BeginDock("Game", false, &visible, false);
	size = ImVec2(App->renderer3D->GetTextureSize());

	ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(),size, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::EndDock();
	return UPDATE_CONTINUE;
}
