#include "Application.h"
#include "PanelConsole.h"
#include "ModuleInput.h"
#include "Brofiler/Brofiler.h"

#include "GL/include/glew.h"


PanelConsole::~PanelConsole()
{
	CreateLOGDoc();

	for (auto it_obj = console_text.begin(); it_obj != console_text.end();) {
		(*it_obj).text.clear();
		it_obj = console_text.erase(it_obj);
	}
	console_text.clear();
}

update_status PanelConsole::Draw() {

	BROFILER_CATEGORY("Draw_PanelConsole", Profiler::Color::GoldenRod);
	update_status ret = UPDATE_CONTINUE;

	CopyLogs();

	ImGui::Begin(name, &enabled);
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

	if (scroll_bottom)
	{
		ImGui::SetScrollHereY(1.0f);
		scroll_bottom = false;
	}

	ImGui::EndChild(); ImGui::Separator();

	ImGui::InputText("Input", filter_text, IM_ARRAYSIZE(filter_text), ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::End();

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

	scroll_bottom = true;
}

void PanelConsole::FilterConsole()
{
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

void PanelConsole::CreateLOGDoc()
{
	FILE *fp = fopen("console_log.txt", "w");
	if (fp != nullptr)
	{
		for (auto it_list = console_text.begin(); it_list != console_text.end(); it_list++) {
			fprintf(fp, (*it_list).text.c_str());
		}
	}

	fclose(fp);
}
