#pragma once
#include "Module.h"
#include "Globals.h"
#include "Panel.h"
#include "PanelConsole.h"
#include "Primitive.h"

#include "Parson/parson.h"
#include <list>

struct PhysMotor3D;

class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool Start();
	update_status Update(float dt);
	bool Save();
	void Log(const char* log_text);

	bool CleanUp();
public:
	ImVec2 r = { 80,100 };
	std::list<Panel*> topbar_panel_list;

	PanelConsole* panel_console = nullptr;

	JSON_Value* settings_doc;
	JSON_Value* credits_doc;

	bool start_console = false;

};
