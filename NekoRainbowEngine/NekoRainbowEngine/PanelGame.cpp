#include "PanelGame.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "Brofiler/Brofiler.h"
#include <string>


update_status PanelGame::Draw()
{
	BROFILER_CATEGORY("Draw_PanelGame", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	size = App->window->GetWinSize();

	ImGui::Checkbox("Grid", &App->viewport->draw_grid); ImGui::SameLine();
	ImGui::Checkbox("Fill", &App->viewport->fill_on); ImGui::SameLine();
	ImGui::Checkbox("Wireframe", &App->viewport->wireframe_on); ImGui::SameLine();
	ImGui::Checkbox("Points", &App->viewport->point_on);

	ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(), ImVec2(size.x*0.65, size.y*0.65), ImVec2(0, 1), ImVec2(1, 0));

	
	Logic();

	ImGui::End();
	return UPDATE_CONTINUE;
}

void PanelGame::Logic()
{

	// Get docking cursor values
	width = ImGui::GetWindowWidth();
	height = ImGui::GetWindowHeight();

	WorldPosX = ImGui::GetWindowPos().x + ImGui::GetCursorPosX();
	WorldPosY = ImGui::GetWindowPos().y + ImGui::GetCursorPosY();


}
