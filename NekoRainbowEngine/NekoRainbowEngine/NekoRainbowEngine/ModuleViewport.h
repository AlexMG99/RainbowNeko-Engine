#pragma once
#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"

class GameObject;

class ModuleViewport : public Module
{
public:
	ModuleViewport(Application* app, bool start_enabled = true);
	~ModuleViewport();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CreateGameObject(GameObject* obj, bool active, GameObject* parent = nullptr);

private:
	void CreateGrid(uint separation, uint lines);

public:
	GameObject* root_object = nullptr;
};
