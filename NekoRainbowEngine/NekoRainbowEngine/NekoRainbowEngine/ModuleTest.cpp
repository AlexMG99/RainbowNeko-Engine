#include "Globals.h"
#include "Application.h"
#include "ModuleTest.h"

#include "SDL/include/SDL_opengl.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "PCG/pcg_random.hpp"

ModuleTest::ModuleTest(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleTest::~ModuleTest()
{}

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		ImGui::SetNextWindowSize(r);
		ImGui::SetNextWindowPos((App->window->GetWinSize()));
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Inspector");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

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
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N")) {};
			if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {};
			if (ImGui::BeginMenu("Open Recent...")) {
				ImGui::MenuItem("Scene1");
				ImGui::EndMenu();
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {};
			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {};
			ImGui::Separator();

			if (ImGui::MenuItem("New Project")) {};
			if (ImGui::MenuItem("Open Project")) {};
			if (ImGui::MenuItem("Save Project")) {};
			ImGui::Separator();

			if (ImGui::MenuItem("Build Settings...", "Ctrl+Shift+B")) {};
			if (ImGui::MenuItem("Build And Run")) {};
			ImGui::Separator();

			if (ImGui::MenuItem("Exit")) {
				return UPDATE_STOP;
			};

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {};
			if (ImGui::MenuItem("Redo", "CTRL+Y")) {};
			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "CTRL+C")) {};
			if (ImGui::MenuItem("Cut", "CTRL+X")) {};
			if (ImGui::MenuItem("Paste", "CTRL+V")) {};
			ImGui::Separator();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}


