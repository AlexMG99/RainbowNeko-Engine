#include "Panel.h"
#include "PanelDebug.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Application.h"
#include <string>

bool Start()
{
	return true;
}

bool PanelDebug::Start()
{
	gl_depth = glIsEnabled(GL_DEPTH);
	gl_cull_face = glIsEnabled(GL_CULL_FACE);
	gl_lighting = glIsEnabled(GL_LIGHTING);
	gl_color_material = glIsEnabled(GL_COLOR_MATERIAL);
	gl_texture_2d = glIsEnabled(GL_TEXTURE_2D);
	gl_fog = glIsEnabled(GL_FOG);
	gl_shade_model = glIsEnabled(GL_SHADE_MODEL);
	
	return true;
}

update_status PanelDebug::Draw()
{
	if (ImGui::BeginMenu(name)) {
		if (ImGui::MenuItem("Renderer"))
			open_renderer = true;
		
		ImGui::EndMenu();
	}

	if (open_renderer)
		RendererWindow();

	return UPDATE_CONTINUE;

}

void PanelDebug::RendererWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("Renderer Window", &open_renderer, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));
		ImGui::SetWindowPos(ImGui::GetWindowPos(), ImGuiCond_FirstUseEver);

		if (gl_depth) glEnable(GL_DEPTH); else glDisable(GL_DEPTH);
			ImGui::Checkbox("GL DEPTH", &gl_depth);
		
		if (gl_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
			ImGui::Checkbox("GL CULL FACE", &gl_cull_face);

		if (gl_lighting) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
			ImGui::Checkbox("GL LIGHTING", &gl_lighting);

		if (gl_color_material) glEnable(GL_COLOR_MATERIAL); else glDisable(GL_COLOR_MATERIAL);
			ImGui::Checkbox("GL COLOR MATERIAL", &gl_color_material);

		if (gl_texture_2d) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
			ImGui::Checkbox("GL TEXTURE 2D", &gl_texture_2d);

		if (gl_fog) glEnable(GL_FOG); else glDisable(GL_FOG);
			ImGui::Checkbox("GL FOG", &gl_fog);

		if (gl_shade_model) glEnable(GL_SHADE_MODEL); else glDisable(GL_SHADE_MODEL);
			ImGui::Checkbox("GL SHADE MODEL", &gl_shade_model);

		ImGui::Separator();

		ImGui::Checkbox("GL Fill", &gl_fill);

		ImGui::Checkbox("GL Wireframe", &gl_lines);

		ImGui::Checkbox("GL Vertex", &gl_points);

		if (gl_fill) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (gl_lines) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (gl_points) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		}

		ImGui::End();
	}

}
