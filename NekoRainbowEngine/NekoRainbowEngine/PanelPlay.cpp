#include "Application.h"
#include "ModuleViewport.h"
#include "PanelPlay.h"
#include "Time.h"
#include "FBO.h"
#include "imgui/imgui.h"
#include "Brofiler/Brofiler.h"

PanelPlay::PanelPlay():Panel()
{
	scene_timer.Start();
}

update_status PanelPlay::Draw()
{
	BROFILER_CATEGORY("Draw_PanelPlay", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	ButtonFunctions();

	ImGui::SameLine();

	ImGui::Text("Real Time: %.3f", scene_timer.ReadSec()); ImGui::SameLine();
	ImGui::Text("Game Time: %.3f", Time::time);
		
	ImGui::Image((ImTextureID)App->viewport->game_fbo->GetTexture(), App->viewport->game_fbo->GetTextureSize(), ImVec2(0, 1), ImVec2(1, 0));

	window_size = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGui::End();
	return UPDATE_CONTINUE;

}

void PanelPlay::ButtonFunctions()
{
	ImGui::Spacing();

	std::string stop_play = Time::running ? "STOP" : "PLAY";
	if (ImGui::Button(stop_play.c_str(), ImVec2(70, 30)))
	{
		Time::running ? App->viewport->Stop_Time() : App->viewport->Play_Time();
	}

	ImGui::SameLine();

	std::string pause_resume_time = Time::paused ? "CONTINUE" : "PAUSE";
	if (ImGui::Button(pause_resume_time.c_str(), ImVec2(70, 30)))
	{
		Time::paused ? Time::Resume() : Time::Pause();
	}

	ImGui::SameLine();

	if (Time::oneframe)
	{
		Time::Pause();
		Time::oneframe = false;
	}

	if (ImGui::Button("One Frame", ImVec2(70, 30)))
	{
		//Move timer one frame
		Time::oneframe = (Time::oneframe == false) ? true : false;

		if (Time::oneframe)
		{
			//Check if its paused
			if (Time::paused)
			{
				//If it is activate resume (timer runs) and then activate pause (timer stops)
				Time::Resume();
				Time::paused = true;
			}
			else
			{
				//If not activate pause (timer stops) and then activate resume (timer starts)
				Time::paused=true;
				Time::Resume();
			}
		}
	}

}