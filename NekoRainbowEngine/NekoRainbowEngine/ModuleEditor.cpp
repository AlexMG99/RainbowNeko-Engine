#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "PanelTopbar.h"
#include "PanelConsole.h"
#include "PanelConfiguration.h"
#include "PanelScene.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"
#include "PanelShape.h"
#include "ModuleViewport.h"
#include "PanelImporter.h"
#include "PanelPlay.h"

#include "SDL/include/SDL_opengl.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "PCG/pcg_random.hpp"
#include "imGuizmo/ImGuizmo.h"

#include "Parson/parson.h"

#include "Brofiler/Brofiler.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
	BROFILER_CATEGORY("Start_ModuleEditor", Profiler::Color::LimeGreen);

	LOG("Loading Editor Scene");
	bool ret = true;

	App->camera->MoveTo(vec3(7.0f, 7.0f, 10.0f));
	App->camera->LookAt(float3(0, 0, 0));

	//Create Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context); // Link Window and Context
	ImGui_ImplOpenGL3_Init(); 
	
	// Initialize Panels
	panel_topbar = new PanelTopbar("Topbar");
	panel_console = new PanelConsole("Console");
	panel_config = new PanelConfiguration("Configuration");
	panel_hierarchy = new PanelHierarchy("Hierarchy");
	panel_inspector = new PanelInspector("Inspector");
	panel_scene = new PanelScene("Game");
	panel_shape = new PanelShape("Shape Creator");
	panel_importer = new PanelImporter("Assets");
	panel_play = new PanelPlay("Play");

	//Start Panels
	panel_topbar->Start();
	panel_console->Start();
	panel_config->Start();
	panel_hierarchy->Start();
	panel_inspector->Start();
	panel_scene->Start();
	panel_shape->Start();
	panel_importer->Start();
	panel_play->Start();

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_ModuleEditor", Profiler::Color::SkyBlue);

	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) 
		ret = Save();

	// Start the Dear ImGui frame
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);

	ImGui::NewFrame();

	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(gizmos);


	App->viewport->GuizControls();
	App->viewport->GuizLogic();

	
	//// Rendering
	//ImGui::Render();
	return ret;
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	BROFILER_CATEGORY("Update_ModuleEditor", Profiler::Color::DeepSkyBlue);
	//Draw Panels
	ret = panel_topbar->Draw();

	DrawPanels();
	/*ImGui::ShowDemoWindow();*/
	return ret;
}

void ModuleEditor::DrawImGui()
{
	BROFILER_CATEGORY("Draw_ModuleEditor_ImGui", Profiler::Color::GoldenRod);

	

	
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor scene");

	//Release Panels
	RELEASE(panel_config);
	RELEASE(panel_scene);
	RELEASE(panel_hierarchy);
	RELEASE(panel_inspector);
	RELEASE(panel_console);
	RELEASE(panel_topbar);
	RELEASE(panel_shape);
	RELEASE(panel_importer);
	RELEASE(panel_play);

	return true;
}

void ModuleEditor::DrawPanels()
{

	BROFILER_CATEGORY("Draw_ModuleEditor_Panels", Profiler::Color::GoldenRod);

	//Dockspace
	ImVec2 display_size = ImGui::GetIO().DisplaySize;
	float offset_y = 19;
	display_size.y = App->window->GetWinSize().y - offset_y;

	//Dockspace
	ImGui::SetNextWindowSize(display_size);
	ImGui::SetNextWindowPos(ImVec2(0, offset_y));

	ImGui::Begin("PanelUp", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	// DockSpace
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if(panel_console->IsEnabled())
		panel_console->Draw();
	if (panel_config->IsEnabled())
		panel_config->Draw();
	if (panel_hierarchy->IsEnabled())
		panel_hierarchy->Draw();
	if (panel_inspector->IsEnabled())
		panel_inspector->Draw();
	if (panel_scene->IsEnabled())
		panel_scene->Draw();
	if (panel_shape->IsEnabled())
		panel_shape->Draw();
	if (panel_importer->IsEnabled())
		panel_importer->Draw();
	if (panel_play->IsEnabled())
		panel_play->Draw();

	ImGui::End();
}

update_status ModuleEditor::Save()
{
	update_status ret = UPDATE_CONTINUE;

	ret = panel_topbar->Save();

	return ret;
}


