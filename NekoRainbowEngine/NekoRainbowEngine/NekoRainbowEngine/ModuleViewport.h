#pragma once
#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"

class ModuleViewport : public Module
{
public:
	ModuleViewport(Application* app, bool start_enabled = true);
	~ModuleViewport();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	void CreateGrid(uint separation, uint lines);
	void makeCheckImage(void);
};
