#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

struct PhysMotor3D;

class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	ImVec2 r = { 80,100 };

};
