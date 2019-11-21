#include "Panel.h"
#include "PanelConsole.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleInput.h"

#include "GL/include/glew.h"


PanelConsole::~PanelConsole()
{
	buffer_text.clear();
}

update_status PanelConsole::Draw() {

	update_status ret = UPDATE_CONTINUE;

	if(ImGui::Begin(name, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		ImGui::SetWindowSize(ImVec2(450, 445));
		if (ImGui::SmallButton("Clear")) 
			Clear();

		ImGui::Separator();

		//Console output
		ImGui::BeginChild("Console Output", ImVec2(0, 295), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::TextUnformatted(buffer_text.begin());
		if (scroll_to_bottom) {
			ImGui::SetScrollHereY(1.0f);
			scroll_to_bottom = false;
		}
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
	scroll_to_bottom = true;
}

void PanelConsole::Clear()
{
	buffer_text.clear();
}
