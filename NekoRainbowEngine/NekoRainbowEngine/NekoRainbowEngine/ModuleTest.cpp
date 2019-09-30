#include "Globals.h"
#include "Application.h"
#include "ModuleTest.h"

#include "SDL/include/SDL_opengl.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "PCG/pcg_random.hpp"

#include "Parson/parson.h"

#include "PanelHelp.h"
#include "PanelFile.h"
#include "PanelEdit.h"
#include "PanelConfig.h"
#include "PanelConsole.h"

ModuleTest::ModuleTest(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleTest::~ModuleTest()
{}

bool ModuleTest::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	topbar_panel_list.push_back(new PanelFile("File"));
	topbar_panel_list.push_back(new PanelEdit("Edit"));
	topbar_panel_list.push_back(new PanelHelp("Help"));
	topbar_panel_list.push_back(new PanelConfig("Configuration"));

	panel_console = new PanelConsole("Console");

	settings_doc = json_parse_file("Settings/win_config.json");
	credits_doc = json_parse_file("Settings/win_about.json");

	if (settings_doc == NULL) {
		settings_doc = json_value_init_object();
	}

	json_serialize_to_file(settings_doc, "Settings/win_config.json");

	if (credits_doc == NULL) {
		credits_doc = json_value_init_object();
	}

	json_serialize_to_file(credits_doc, "Settings/win_about.json");

	for (std::list<Panel*>::iterator item = topbar_panel_list.begin(); ((item != topbar_panel_list.end()) && (ret == UPDATE_CONTINUE)); item++) {
		ret = (*item)->Start();
	}

	ret = panel_console->Start();


	return ret;
}

// Load assets
bool ModuleTest::CleanUp()
{
	LOG("Unloading Intro scene");

	json_value_free(settings_doc);
	json_value_free(credits_doc);
	return true;
}

bool ModuleTest::Save()
{
	bool ret = false;

	for (std::list<Panel*>::iterator item = topbar_panel_list.begin(); ((item != topbar_panel_list.end())); item++) {
		ret = (*item)->Save();
	}

	return ret;
}

void ModuleTest::Log(const char * log_text)
{
	if (panel_console != nullptr) {
		if (!start_console) {
			panel_console->AddLog(App->GetLog());
			start_console = true;
		}
		else {
			panel_console->AddLog(log_text);
		}
	}

}

// Update
update_status ModuleTest::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Top bar
	ImGui::BeginMainMenuBar();
	for (std::list<Panel*>::iterator item = topbar_panel_list.begin(); ((item != topbar_panel_list.end()) && (ret == UPDATE_CONTINUE)); item++) {
		ret = (*item)->Draw();
	}
	ImGui::EndMainMenuBar();

	ret = panel_console->Draw();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		Save();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}


