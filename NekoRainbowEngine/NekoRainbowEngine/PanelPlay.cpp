#include "PanelPlay.h"
#include "Application.h"
#include "Time.h"
#include "imgui/imgui.h"
#include "Brofiler/Brofiler.h"

update_status PanelPlay::Draw()
{
	BROFILER_CATEGORY("Draw_PanelPlay", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	/*ImGui::SetCursorPos({ (ImGui::GetWindowWidth()*0.5), ImGui::GetCursorPosY() });*/

	//ButtonFunctions();

	//ImGui::SameLine();

	//ImGui::Text("Real Time: %.3f", App->MS_Timer()); ImGui::SameLine();
	//ImGui::Text("Game Time: %.3f", Time::time);
	//	


	ImGui::End();
	return UPDATE_CONTINUE;

}

//void PanelPlay::ButtonFunctions()
//{
//	std::string stop_play = Time::running ? "STOP" : "PLAY";
//	if (ImGui::Button(stop_play.c_str(), ImVec2(70, 30)))
//	{
//		Time::running ? App->viewport->StopTime() : App->viewport->Start();
//	}
//	ImGui::SameLine();
//
//
//	std::string pause_resume_time = Time::paused ? "RESUME" : "PAUSE";
//	if (ImGui::Button(pause_resume_time.c_str(), ImVec2(70, 30)))
//	{
//		Time::paused ? Time::Resume() : Time::Pause();
//	}
//
//	ImGui::SameLine();
//
//	if (ImGui::Button("One Frame", ImVec2(70, 30)))
//	{
//
//	}
//
//}