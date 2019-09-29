#include "Panel.h"
#include "PanelHelp.h"

#include "imgui/imgui.h"
#include "Application.h"

#include <string>

bool PanelHelp::Start()
{
	JSON_Object* abo = json_object(App->scene_test->credits_doc);

	

	//Set Help Attributes
	strcpy_s(App->window->documentation, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "Help")), "Documentation"));
	strcpy_s(App->window->download_last, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "Help")), "Download Last"));
	strcpy_s(App->window->report_bug, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "Help")), "Report Bug"));

	
	

	//Set Credis Attributes
	strcpy_s(App->window->engine_name, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "Engine Name"));
	strcpy_s(App->window->authors, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "Authors"));
	strcpy_s(App->window->description, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "Description"));
	strcpy_s(App->window->githublink, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "Github Link"));
	strcpy_s(App->window->SDL_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "SDL Link"));
	strcpy_s(App->window->ImGui_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "ImGui Link"));
	strcpy_s(App->window->Glew_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "Glew Link"));
	strcpy_s(App->window->OpenGL_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "OpenGL Link"));
	strcpy_s(App->window->MathGeoLib_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "MathGeoLib Link"));
	strcpy_s(App->window->PCG_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "PCG Link"));
	strcpy_s(App->window->Parson_link, json_object_get_string(json_object_get_object(json_object_get_object(abo, "Credits"), (abo, "About")), "Parson Link"));

	return true;
}

update_status PanelHelp::Draw() {
	//Documentation Links & About
	if (ImGui::BeginMenu(name))
	{
		if (ImGui::MenuItem("Github repository\t"))
			RequestBrowser(App->window->githublink);
		if (ImGui::MenuItem("Documentation\t"))
			RequestBrowser(App->window->documentation);
		if (ImGui::MenuItem("Download latest\t"))
			RequestBrowser(App->window->download_last);
		if (ImGui::MenuItem("Report a bug\t"))
			RequestBrowser(App->window->report_bug);
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
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("About", &open, window_flags)) {

		ImGui::Text("%s", App->window->engine_name);
		ImGui::Separator();

		ImGui::Text("%s", App->window->description);

		ImGui::Text("%s", App->window->authors); 
		if (ImGui::Button("Github repository"))
			RequestBrowser(App->window->githublink);

		ImGui::Separator();
		ImGui::Text("Libraries used in our engine:");

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("SDL 2.0"))
		RequestBrowser(App->window->SDL_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("ImGui 1.73"))
			RequestBrowser(App->window->ImGui_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("Glew 2.1.0"))
			RequestBrowser(App->window->Glew_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("OpenGL"))
			RequestBrowser(App->window->OpenGL_link);

		ImGui::Bullet(); ImGui::SameLine(); if(ImGui::MenuItem("MatGeoLib 1.5"))
			RequestBrowser(App->window->MathGeoLib_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("PCG 0.98.1"))
			RequestBrowser(App->window->PCG_link);

		ImGui::Bullet(); ImGui::SameLine(); if (ImGui::MenuItem("Parson"))
			RequestBrowser(App->window->Parson_link);
		ImGui::Separator();
		


		ImGui::End();
	}
}

