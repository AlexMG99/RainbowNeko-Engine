#pragma once
#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"


class ModuleViewport : public Module
{
public:
	ModuleViewport(Application* app, bool start_enabled = true);
	~ModuleViewport();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr, float3 position = { 0.0f,0.0f,0.0f } , float3 scale = { 0.0f,0.0f,0.0f }, Quat rotation = Quat::identity);
	void DeleteGameObject();

	void CheckObjectSelected(GameObject* obj);

private:
	void CreateGrid(uint separation, uint lines);

public:
	GameObject* root_object = nullptr;
	GameObject* selected_object = nullptr;
	uint shape_num = 0;
};
