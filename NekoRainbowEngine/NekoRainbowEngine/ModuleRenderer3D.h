#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "Light.h"

#define MAX_LIGHTS 8

class ImVec2;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat4x4 ProjectionMatrix;
};