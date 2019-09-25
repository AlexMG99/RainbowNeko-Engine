#pragma once
#include "Module.h"
#include "Globals.h"
#include "Panel.h"
#include <list>

struct PhysMotor3D;

class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Save();
public:
	ImVec2 r = { 80,100 };
	std::list<Panel*> topbar_panel_list;

	std::list<Panel*> panel_list;


};
