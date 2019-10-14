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
#include "PanelWindow.h"
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

	/*ImGui::StyleColorsDark();*/

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	/*topbar_panel_list.push_back(new PanelFile("File"));
	topbar_panel_list.push_back(new PanelHelp("Help"));
	topbar_panel_list.push_back(new PanelDebug("Debug"));
	topbar_panel_list.push_back(new PanelWindow("Window"));

	panel_console = new PanelConsole("Console");
	panel_configuration = new PanelConfiguration("Configuration");
	panel_console->Start();
	panel_configuration->Start();

	*/

	settings_doc = json_parse_file("Settings/win_config.json");
	credits_doc = json_parse_file("Settings/win_about.json");

	if (!settings_doc) {
		settings_doc = json_value_init_object();
		json_serialize_to_file(settings_doc, "Settings/win_config.json");
	}

	if (!credits_doc) {
		credits_doc = json_value_init_object();
		json_serialize_to_file(credits_doc, "Settings/win_about.json");
	}

	panel_topbar = new PanelTopbar();

	panel_topbar->Start();

	return ret;
}

update_status ModuleTest::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) 
		ret = Save();

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

update_status ModuleTest::Save()
{
	update_status ret = UPDATE_CONTINUE;

	panel_topbar->Save();

	return ret;
}

void ModuleTest::Log(const char * log_text)
{
	if (panel_topbar && panel_topbar->panel_console) {
		if (!panel_topbar->panel_console->start_console) {
			panel_topbar->panel_console->AddLog(App->GetLog());
			panel_topbar->panel_console->start_console = true;
		}
		else {
			panel_topbar->panel_console->AddLog(log_text);
		}
	}

}

// Update
update_status ModuleTest::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	panel_topbar->Draw();

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}


