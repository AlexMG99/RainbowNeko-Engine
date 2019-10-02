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

bool PanelConsole::Start()
{
	return true;
}

update_status PanelConsole::Draw() {

	update_status ret = UPDATE_CONTINUE;

	if(ImGui::Begin(name, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		ImGui::SetWindowSize(ImVec2(450, 445));
		if (ImGui::SmallButton("Clear")) 
			Clear();

		ImGui::Separator();

		// gl enum buttons
		ImGui::Text("GLEnum buttons:");
		
		if (ImGui::Button("GL Depth")) {
			SetGLProperty(GL_DEPTH_TEST);
			LOG("GL DEPTH TEST: %s", glIsEnabled(GL_DEPTH_TEST) ? "true" : "false");
		} ImGui::SameLine();

		if (ImGui::Button("GL Cullface")) {
			SetGLProperty(GL_CULL_FACE);
			LOG("GL CULL FACE: %s", glIsEnabled(GL_CULL_FACE) ? "true" : "false");
		} ImGui::SameLine();

		if (ImGui::Button("GL Lighting")) {
			SetGLProperty(GL_LIGHTING);
			LOG("GL LIGHTING: %s", glIsEnabled(GL_LIGHTING) ? "true" : "false");
		} ImGui::SameLine();

		if (ImGui::Button("GL Color Material")) {
			SetGLProperty(GL_COLOR_MATERIAL);
			LOG("GL COLOR MATERIAL: %s", glIsEnabled(GL_COLOR_MATERIAL) ? "true" : "false");
		}

		if (ImGui::Button("GL Texture 2D")) {
			SetGLProperty(GL_TEXTURE_2D);
			LOG("GL TEXTURE 2D: %s", glIsEnabled(GL_TEXTURE_2D) ? "true" : "false");
		} ImGui::SameLine();

		if (ImGui::Button("GL Fog")) {
			SetGLProperty(GL_FOG);
			LOG("GL FOG: %s", glIsEnabled(GL_FOG) ? "true" : "false");
		} ImGui::SameLine();

		if (ImGui::Button("GL Shade Model")) {
			SetGLProperty(GL_SHADE_MODEL);
			LOG("GL SHADE MODEL: %s", glIsEnabled(GL_SHADE_MODEL) ? "true" : "false");
		}

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

void PanelConsole::SetGLProperty(int hex_value)
{
	glIsEnabled((GLenum)hex_value) ? glDisable((GLenum)hex_value) : glEnable((GLenum)hex_value);
}

void PanelConsole::Clear()
{
	buffer_text.clear();
}
