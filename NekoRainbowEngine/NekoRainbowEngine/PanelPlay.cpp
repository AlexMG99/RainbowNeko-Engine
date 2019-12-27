#include "Application.h"
#include "ModuleViewport.h"
#include "PanelPlay.h"
#include "FBO.h"
#include "imgui/imgui.h"
#include "Brofiler/Brofiler.h"
#include "Time.h"

PanelPlay::PanelPlay() :Panel()
{
	scene_timer.Start();
}


update_status PanelPlay::Draw()
{
	BROFILER_CATEGORY("Draw_PanelPlay", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	cursor_pos.x = App->input->GetMouseX() - ImGui::GetCursorScreenPos().x - ImGui::GetWindowWidth();
	cursor_pos.y = App->input->GetMouseY() - ImGui::GetCursorScreenPos().y - ImGui::GetWindowHeight() - 20;
	ImGui::SetCursorPos({ (float)(ImGui::GetWindowWidth()*0.35) , ImGui::GetCursorPosY() });

	ImGui::Text("Real Time: %.3f", scene_timer.ReadSec()); ImGui::SameLine();
	ImGui::Text("Game Time: %.3f", Time::time);

	ImGui::Image((ImTextureID)App->viewport->game_fbo->GetTexture(), App->viewport->game_fbo->GetTextureSize(), ImVec2(0, 1), ImVec2(1, 0));
	window_size = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	if (ImGui::IsWindowHovered())
		App->viewport->is_over_game = true;

	ImGui::End();

	return UPDATE_CONTINUE;

}