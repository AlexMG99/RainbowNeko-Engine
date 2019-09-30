#include "Panel.h"
#include "PanelConsole.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleInput.h"

#include <gl/GL.h>


bool PanelConsole::Start()
{
	return true;
}

update_status PanelConsole::Draw() {

	update_status ret = UPDATE_CONTINUE;

	if(ImGui::Begin(name, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		if (ImGui::SmallButton("Add text"))
			AddLog("Hamborguesa\n");
		if (ImGui::SmallButton("Clear"))
			Clear();

		ImGui::Separator();
		ImGui::BeginChild("Console Output", ImVec2(600, 295));
		ImGui::TextUnformatted(buffer_text.begin());
		ImGui::SetScrollHere();
		ImGui::EndChild(); ImGui::Separator();

		if (ImGui::InputText("Input", console_input, IM_ARRAYSIZE(console_input), ImGuiInputTextFlags_EnterReturnsTrue) ) {
			AddLog(console_input);
			AddLog("\n");
		}
		ImGui::End();
	}

	return ret;
}

void PanelConsole::AddLog(const char * text)
{
	buffer_text.appendf(text);
}

void PanelConsole::Clear()
{
	buffer_text.clear();
}
