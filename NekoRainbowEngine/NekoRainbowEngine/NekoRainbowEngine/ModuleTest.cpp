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

	//Create Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context); // Link Window and Context
	ImGui_ImplOpenGL3_Init(); // Initialize Panels

	panel_topbar = new PanelTopbar();

	panel_topbar->Start();

	return ret;
}

update_status ModuleTest::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) 
		ret = Save();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return ret;
}

update_status ModuleTest::Update(float dt)
{
	panel_topbar->Draw();
	return UPDATE_CONTINUE;
}

void ModuleTest::DrawImGui()
{
	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Load assets
bool ModuleTest::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleTest::Save()
{
	update_status ret = UPDATE_CONTINUE;

	panel_topbar->Save();

	return ret;
}

// Update
update_status ModuleTest::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}


