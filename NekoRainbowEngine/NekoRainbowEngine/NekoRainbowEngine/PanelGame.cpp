#include "Panel.h"
#include "PanelGame.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Application.h"
#include <string>

update_status PanelGame::Draw()
{
	size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

	ImGui::Begin(name);
	ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(),ImVec2(1280,1024), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	return UPDATE_CONTINUE;
}
