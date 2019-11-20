#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "FBO.h"

#include "Light.h"

#define MAX_LIGHTS 8

class ImVec2;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	ImVec2 GetTextureSize() const;
	uint GetWinTexture() const;

	void UpdateProjectionMatrix();


public:
	FBO* fbo = nullptr;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};