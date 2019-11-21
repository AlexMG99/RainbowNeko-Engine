#include "Application.h"
#include "ModuleViewport.h"
#include "PanelPlay.h"
#include "FBO.h"
#include "imgui/imgui.h"
#include "Brofiler/Brofiler.h"

update_status PanelPlay::Draw()
{
	BROFILER_CATEGORY("Draw_PanelPlay", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
		
	ImGui::Image((ImTextureID)App->viewport->game_fbo->GetTexture(), App->viewport->game_fbo->GetTextureSize(), ImVec2(0, 1), ImVec2(1, 0));

	window_size = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGui::End();
	return UPDATE_CONTINUE;

}