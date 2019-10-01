#include "Panel.h"
#include "PanelConsole.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleInput.h"

#include "GL/include/glew.h"


bool PanelConsole::Start()
{
	return true;
}

update_status PanelConsole::Draw() {

	update_status ret = UPDATE_CONTINUE;

	if(ImGui::Begin(name, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		ImGui::SetWindowSize(ImVec2(550, 435));
		if (ImGui::SmallButton("Clear"))
			Clear();

		ImGui::Separator();

		// gl enum buttons
		ImGui::Text("GLEnum buttons:");

		if (ImGui::SmallButton("GL Depth")) {
			SetGLProperty(GL_DEPTH_TEST);
			LOG("GL_DEPTH_TEST: %i",glIsEnabled(GL_DEPTH_TEST));
		} ImGui::SameLine();

		if (ImGui::SmallButton("GL Cullface")) {
			SetGLProperty(GL_CULL_FACE);
			LOG("GL_CULL_FACE: %i", glIsEnabled(GL_CULL_FACE));
		} ImGui::SameLine();

		if (ImGui::SmallButton("GL Lighting")) {
			SetGLProperty(GL_LIGHTING);
			LOG("GL_LIGHTING: %i", glIsEnabled(GL_LIGHTING));
		} 

		if (ImGui::SmallButton("GL Color Material")) {
			SetGLProperty(GL_COLOR_MATERIAL);
			LOG("GL_COLOR_MATERIAL: %i", glIsEnabled(GL_COLOR_MATERIAL));
		} ImGui::SameLine();

		if (ImGui::SmallButton("GL Texture 2D")) {
			SetGLProperty(GL_TEXTURE_2D);
			LOG("GL_TEXTURE_2D: %i", glIsEnabled(GL_TEXTURE_2D));
		} 

		ImGui::Separator();

		//Console output
		ImGui::BeginChild("Console Output", ImVec2(600, 295));
		ImGui::TextUnformatted(buffer_text.begin());
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

void PanelConsole::SetGLProperty(int hex_value)
{
	glIsEnabled((GLenum)hex_value) ? glDisable((GLenum)hex_value) : glEnable((GLenum)hex_value);
}

void PanelConsole::Clear()
{
	buffer_text.clear();
}