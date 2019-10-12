#pragma once
#include "Module.h"
#include "Globals.h"
#include "Panel.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelConfiguration.h"

#include "Parson/parson.h"
#include <list>

class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	update_status Save();
	void Log(const char* log_text);

	bool CleanUp();
public:
	ImVec2 r = { 80,100 };
	std::list<Panel*> topbar_panel_list;

	PanelInspector* panel_inspector = nullptr;
	PanelConsole*	panel_console = nullptr;
	PanelConfiguration* panel_configuration = nullptr;

	JSON_Value* settings_doc;
	JSON_Value* credits_doc;

};
