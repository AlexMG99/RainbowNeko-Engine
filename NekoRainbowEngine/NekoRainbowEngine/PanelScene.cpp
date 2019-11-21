#include "PanelScene.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "imGuizmo/ImGuizmo.h"
#include "ModuleViewport.h"
#include "FBO.h"
#include "Brofiler/Brofiler.h"
#include <string>


update_status PanelScene::Draw()
{
	BROFILER_CATEGORY("Draw_PanelScene", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	/*size = App->window->GetWinSize();*/

	ImGui::Checkbox("Grid", &App->viewport->draw_grid); ImGui::SameLine();
	ImGui::Checkbox("Fill", &App->viewport->fill_on); ImGui::SameLine();
	ImGui::Checkbox("Wireframe", &App->viewport->wireframe_on); ImGui::SameLine();
	ImGui::Checkbox("Points", &App->viewport->point_on); ImGui::SameLine();
	ImGui::Checkbox("Camera Culling", &App->viewport->camera_culling); ImGui::SameLine();
	ImGui::Checkbox("Gizmos", &App->editor->gizmos);

	WorldPosX = ImGui::GetWindowPos().x + ImGui::GetCursorPosX();
	WorldPosY = ImGui::GetWindowPos().y + ImGui::GetCursorPosY();

	ImGui::Image((ImTextureID)App->viewport->scene_fbo->GetTexture(), App->viewport->scene_fbo->GetTextureSize(), ImVec2(0, 1), ImVec2(1, 0));

	window_size = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - App->viewport->scene_fbo->GetTextureSize().x) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() - App->viewport->scene_fbo->GetTextureSize().y) * 0.5f);

	App->viewport->GuizControls();
	App->viewport->GuizLogic();
	

	ImGui::End();
	return UPDATE_CONTINUE;
}
