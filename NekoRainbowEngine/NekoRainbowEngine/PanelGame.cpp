#include "PanelGame.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "imGuizmo/ImGuizmo.h"
#include "ModuleViewport.h"
#include "FBO.h"
#include "Brofiler/Brofiler.h"
#include <string>


update_status PanelGame::Draw()
{
	BROFILER_CATEGORY("Draw_PanelGame", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	/*size = App->window->GetWinSize();*/

	ImGui::Checkbox("Grid", &App->viewport->draw_grid); ImGui::SameLine();
	ImGui::Checkbox("Fill", &App->viewport->fill_on); ImGui::SameLine();
	ImGui::Checkbox("Wireframe", &App->viewport->wireframe_on); ImGui::SameLine();
	ImGui::Checkbox("Points", &App->viewport->point_on); ImGui::SameLine();
	ImGui::Checkbox("Camera Culling", &App->viewport->camera_culling);

	WorldPosX = ImGui::GetWindowPos().x + ImGui::GetCursorPosX();
	WorldPosY = ImGui::GetWindowPos().y + ImGui::GetCursorPosY();

	
	ImGui::Image((ImTextureID)App->viewport->scene_fbo->GetTexture(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - width) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() - height) * 0.5f);

	App->viewport->GuizControls();
	App->viewport->GuizLogic();
	

	ImGui::End();
	return UPDATE_CONTINUE;
}
