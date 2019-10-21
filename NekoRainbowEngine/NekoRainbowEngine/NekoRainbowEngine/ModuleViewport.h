#pragma once
#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"
#include "GameObject.h"
#include "ModuleImporter.h"

class ModuleViewport : public Module
{
public:
	ModuleViewport(Application* app, bool start_enabled = true);
	~ModuleViewport();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void AddGameObject(GameObject* obj, object_type type = OBJECT_FBX, bool active = true, GameObject* parent = nullptr);
	void CreateGameObjectShape(object_type type, shape_type s_type, uint slice = 10, uint stack = 10, bool active = true, GameObject* parent = nullptr);
	void DeleteGameObject();

	void CheckObjectSelected(GameObject* obj);

private:
	void CreateGrid(uint separation, uint lines);

public:
	GameObject* root_object = nullptr;
	uint shape_num = 0;
};
