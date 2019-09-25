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

	panel_list.push_back(new PanelFile("File"));
	panel_list.push_back(new PanelEdit("Edit"));
	panel_list.push_back(new PanelHelp("Help"));

	return ret;
}

// Load assets
bool ModuleTest::CleanUp()
{
	LOG("Unloading Intro scene");

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

	//Documentation Menu

	if (ImGui::BeginMenu("Documentation"))
	{
		if (ImGui::MenuItem("Open Github repository"))
			App->RequestBrowser("https://github.com/AlexMG99/RainbowNeko-Engine");
		if (ImGui::MenuItem("Download Latest"))
			App->RequestBrowser("");
		ImGui::EndMenu();
	}

	// Top bar
	{
		ImGui::BeginMainMenuBar();
		for (std::list<Panel*>::iterator item = panel_list.begin(); ((item != panel_list.end()) && (ret == UPDATE_CONTINUE)); item++) {
			ret = (*item)->Draw();
		}
		ImGui::EndMainMenuBar();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}


