#include "Panel.h"
#include "PanelConsole.h"
#include "imgui/imgui.h"

#include <gl/GL.h>


bool PanelConsole::Start()
{
	buffer_text.size();
	return true;
}

update_status PanelConsole::Draw() {

	update_status ret = UPDATE_CONTINUE;

	if(ImGui::Begin(name, &open)) {
		if (ImGui::SmallButton("Add text"))
			AddLog("Hamborguesa\n");
		if (ImGui::SmallButton("Clear"))
			Clear();
		ImGui::TextUnformatted(buffer_text.begin());
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
