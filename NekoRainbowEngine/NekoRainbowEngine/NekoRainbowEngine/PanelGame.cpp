#include "Panel.h"
#include "PanelGame.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Application.h"
#include <string>

update_status PanelGame::Draw()
{

	ImGui::Begin(name);
	//ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(), { 200,200 });
	ImGui::End();
	return UPDATE_CONTINUE;
}
