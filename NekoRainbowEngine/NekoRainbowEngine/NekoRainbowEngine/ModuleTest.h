#pragma once
#include "Module.h"
#include "Globals.h"
#include "Panel.h"
#include "PanelTopbar.h"
#include <list>

class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	update_status Save();

	void DrawImGui();
	bool CleanUp();
public:
	PanelTopbar* panel_topbar = nullptr;

};
