#include "Application.h"
#include "PanelConsole.h"
#include "ModuleInput.h"

#include "GL/include/glew.h"
#include "imgui/imgui_docking.h"


PanelConsole::~PanelConsole()
{

}

update_status PanelConsole::Draw() {

	update_status ret = UPDATE_CONTINUE;

	CopyLogs();

	ImGui::BeginDock("Console", false, &visible, true);
	//if(ImGui::Begin(name, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
	//	ImGui::SetWindowSize(ImVec2(450, 445));
		//if (ImGui::SmallButton("Clear")) 
		//	Clear();

		ImGui::Separator();

		//Console output
		ImGui::BeginChild("Console Output", ImVec2(0, 165), false, ImGuiWindowFlags_HorizontalScrollbar);
		for (auto it_cons = console_text.begin(); it_cons != console_text.end(); it_cons++) 
		{
			ImVec4 color = GetColorFromType((*it_cons).type);
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::Text((*it_cons).text.c_str());
			ImGui::PopStyleColor();
		}
		ImGui::EndChild(); ImGui::Separator();

		if (ImGui::InputText("Input", console_input, IM_ARRAYSIZE(console_input), ImGuiInputTextFlags_EnterReturnsTrue) ) {
			AddLog(console_input);
		}
		ImGui::EndDock();
		/*ImGui::End();
	}*/

	return ret;
}

void PanelConsole::AddLog(const char * text)
{
	AddLog(ConsoleText(text, CONSOLE_DEFAULT));
}

void PanelConsole::AddLog(ConsoleText text)
{
	console_text.push_back(text);

	if (console_text.size() > MAX_LINES) {
		console_text.pop_front();
	}

	/*ImGui::SetScrollHereY(1.0f);*/
}

ImVec4 PanelConsole::GetColorFromType(ConsoleTextType type)
{
	ImVec4 color = { 255,255,255,255 };

	switch(type)
	{
	case CONSOLE_DEFAULT:
		color = { 255,255,255,255 };
		break;
	case CONSOLE_INFO:
		color = { 255,255,255,255 };
		break;
	case CONSOLE_SUCCESS:
		color = { 0,18,71,255 };
		break;
	case CONSOLE_WARNING:
		color = { 248,224,90,255 };
		break;
	case CONSOLE_ERROR:
		color = { 228,22,22,255 };
		break;
	}

	return color;
}

void PanelConsole::CopyLogs()
{
	std::list<ConsoleText> logs = App->GetLogs();

	for (auto it_log = logs.begin(); it_log != logs.end(); it_log++)
	{
		AddLog((*it_log));
	}

	App->ClearLogs();
}
