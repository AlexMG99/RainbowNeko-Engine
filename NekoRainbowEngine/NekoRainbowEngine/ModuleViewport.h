#pragma once
#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class Scene;

class ModuleViewport : public Module
{
public:
	ModuleViewport(Application* app, bool start_enabled = true);
	~ModuleViewport();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadScene() { return true; };

	bool SaveScene(GameObject * obj);
	bool SaveGameObject(GameObject* obj);

	bool ResetScene() { return true; };

	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr, float3 position = { 0.0f,0.0f,0.0f } , float3 scale = { 1.0f,1.0f,1.0f }, Quat rotation = Quat::identity);
	void DeleteGameObject();

private:
	void DrawGrid(uint separation, uint lines);

public:
	Scene* scene = nullptr;
	GameObject* root_object = nullptr;
	GameObject* camera_test = nullptr;
	GameObject* selected_object = nullptr;

	bool draw_grid = true;
	bool wireframe_on = false;
	bool fill_on = true;
	bool point_on = false;

};
