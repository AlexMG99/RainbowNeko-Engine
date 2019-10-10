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
	void CreateGrid();

public:
	uint num_vertices = 36; //6 faces, 2 triangles by face, 3 vertex per triangle 6*2*3 = 36
	uint my_id;
	uint my_indices;

	uint separation = 2;
	uint lines = 20;
};

void makeCheckImage(void);
