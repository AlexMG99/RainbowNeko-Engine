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

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	topbar_panel_list.push_back(new PanelFile("File"));
	topbar_panel_list.push_back(new PanelEdit("Edit"));
	topbar_panel_list.push_back(new PanelHelp("Help"));

	topbar_panel_list.push_back(new PanelConfig("Configuration"));

	return ret;
}

// Load assets
bool ModuleTest::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

bool ModuleTest::Save()
{
	JSON_Value *win_config = json_parse_file("Settings/win_config.json");

	if (win_config == NULL) {
		win_config = json_value_init_object();
	}

	JSON_Object* win_object = json_object(win_config);
	json_object_set_string(win_object, "Application", "Tortilla");
	
	json_serialize_to_file(win_config, "Settings/win_config.json");
	json_value_free(win_config);

	return true;
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

	for (std::list<Panel*>::iterator item = panel_list.begin(); ((item != panel_list.end()) && (ret == UPDATE_CONTINUE)); item++) {
		ret = (*item)->Draw();
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		Save();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}


