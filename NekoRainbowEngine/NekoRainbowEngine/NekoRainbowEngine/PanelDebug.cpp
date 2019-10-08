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


	gl_cull_face = SetGLProperty(GL_CULL_FACE);
	gl_lighting = SetGLProperty(GL_LIGHTING);
	gl_color_material = SetGLProperty(GL_COLOR_MATERIAL);
	gl_texture_2d = SetGLProperty(GL_TEXTURE_2D);
	gl_fog = SetGLProperty(GL_FOG);
	gl_shade_model = SetGLProperty(GL_SHADE_MODEL);
	gl_front_and_back = SetGLProperty(GL_FRONT_AND_BACK);
	
	return true;
}

update_status PanelDebug::Draw()
{
	if (ImGui::BeginMenu(name)) {
		if (ImGui::MenuItem("Console") && !open)
			open = true;
		if (ImGui::MenuItem("Renderer") && !open)
			open = true;
		if (ImGui::MenuItem("Save"));
		ImGui::EndMenu();
	}

	if (open)
		RendererWindow();

	return UPDATE_CONTINUE;

}

update_status PanelDebug::RendererWindow()
{

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("Renderer Window", &open, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));
		ImGui::SetWindowPos(ImGui::GetWindowPos(), ImGuiCond_FirstUseEver);
		
	
		ImGui::Checkbox("GL DEPTH", &gl_depth);

		ImGui::Checkbox("GL CULL FACE", &gl_cull_face);

		ImGui::Checkbox("GL LIGHTING", &gl_lighting);

		ImGui::Checkbox("GL COLOR MATERIAL", &gl_color_material);

		ImGui::Checkbox("GL TEXTURE 2D", &gl_texture_2d);

		ImGui::Checkbox("GL FOG", &gl_fog);

		ImGui::Checkbox("GL SHADE MODEL", &gl_shade_model);

		ImGui::Checkbox("WIREFRAME", &gl_front_and_back);


		ImGui::End();
	}
	return UPDATE_CONTINUE;
}

bool PanelDebug::SetGLProperty(int hex_value)
{
	
	if (glIsEnabled((GLenum)hex_value) == true) {
		return true;
	}
	else if (glIsEnabled((GLenum)hex_value) == false) {
		return false;
	}
}
