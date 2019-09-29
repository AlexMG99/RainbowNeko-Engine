#include "Panel.h"
#include "PanelHelp.h"

#include "imgui/imgui.h"
#include "Application.h"

#include <string>

bool PanelHelp::Start()
{
	JSON_Object* about = json_object(App->scene_test->credits_doc);

	//Set Help Attributes
	strcpy_s(documentation, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "Help")), "Documentation"));
	strcpy_s(download_last, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "Help")), "Download Last"));
	strcpy_s(report_bug, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "Help")), "Report Bug"));

	//Set Credis Attributes
	strcpy_s(engine_name, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "Engine Name"));
	strcpy_s(authors, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "Authors"));
	strcpy_s(description, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "Description"));
	strcpy_s(githublink, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "Github Link"));
	strcpy_s(SDL_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "SDL Link"));
	strcpy_s(ImGui_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "ImGui Link"));
	strcpy_s(Glew_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "Glew Link"));
	strcpy_s(OpenGL_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "OpenGL Link"));
	strcpy_s(MathGeoLib_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "MathGeoLib Link"));
	strcpy_s(PCG_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "PCG Link"));
	strcpy_s(Parson_link, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "Parson Link"));
	strcpy_s(license, json_object_get_string(json_object_get_object(json_object_get_object(about, "Credits"), (about, "About")), "License"));

	return true;
}

update_status PanelHelp::Draw() {
	//Documentation Links & About
	if (ImGui::BeginMenu(name))
	{
		if (ImGui::MenuItem("Github repository\t"))
			RequestBrowser(githublink);
		if (ImGui::MenuItem("Documentation\t"))
			RequestBrowser(documentation);
		if (ImGui::MenuItem("Download latest\t"))
			RequestBrowser(download_last);
		if (ImGui::MenuItem("Report a bug\t"))
			RequestBrowser(report_bug);
		if (ImGui::MenuItem("About\t") && !open)
			open = true;
		ImGui::EndMenu();
	}

	ImGui::ShowDemoWindow();

	//Checks About
	if(open)
		AboutWindow();

	return UPDATE_CONTINUE;
}

void PanelHelp::RequestBrowser(const char* link)
{
	ShellExecuteA(0, "open", link, 0, 0, 0);
}

void PanelHelp::AboutWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("About", &open, window_flags)) {
		ImGui::SetWindowSize(ImVec2(575, 600));
		ImGui::SetWindowPos(ImGui::GetWindowPos(), ImGuiCond_FirstUseEver);
		ImGui::Text("%s", engine_name);
		ImGui::Separator();

		ImGui::Text("%s", description);

		ImGui::Text("%s", authors); 
		if (ImGui::Button("Github repository"))
			RequestBrowser(githublink);

		ImGui::Separator();
		ImGui::Text("Libraries used in our engine:");

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("SDL 2.0"))
		RequestBrowser(SDL_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("ImGui 1.73"))
			RequestBrowser(ImGui_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("Glew 2.1.0"))
			RequestBrowser(Glew_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("OpenGL"))
			RequestBrowser(OpenGL_link);

		ImGui::Bullet(); ImGui::SameLine(); if(ImGui::MenuItem("MatGeoLib 1.5"))
			RequestBrowser(MathGeoLib_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("PCG 0.98.1"))
			RequestBrowser(PCG_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("Parson"))
			RequestBrowser(Parson_link);
		ImGui::Separator();

		ImGui::Text(license);
		
		ImGui::End();
	}
}

