#pragma once

#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "RayCast.h"
#include "imgui/imgui.h"
#include "imGuizmo/ImGuizmo.h"
#include "Quadtree.h"

class Scene;
class FBO;

class ModuleViewport : public Module
{
public:
	ModuleViewport(Application* app, bool start_enabled = true);
	~ModuleViewport();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	//----Intersections----//
	//bool CompareRayCast(RayCast& a, RayCast & b);
	GameObject* MyRayCastIntersection(LineSegment* ray, RayCast& hit);

	void BoxIntersection(GameObject* obj, LineSegment* ray, std::vector<RayCast>& scene_obj);
	GameObject* TriangleTest(LineSegment& ray, GameObject* obj );

	bool LoadScene(const char* name);
	bool LoadGameObject(Scene scn);

	bool SaveScene();
	bool SaveGameObject(Scene go_scene, GameObject* obj, int* i);

	void ReorganizeHierarchy();

	bool ResetScene();

	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr, float3 position = { 0.0f,0.0f,0.0f } , float3 scale = { 1.0f,1.0f,1.0f }, Quat rotation = Quat::identity);
	GameObject* CreateUIElement(char* name, UI_type type, uint width, uint height, ComponentCanvas* canvas, char* str, GameObject* parent = nullptr, float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f }, Quat rotation = Quat::identity);


	void DeleteGameObject();

public:
	void DrawGrid(uint separation, uint lines);
	void GuizControls();
	void GuizLogic();

	void Play_Time();
	void Stop_Time();

public:
	Scene* scene = nullptr;
	GameObject* root_object = nullptr;
	GameObject* camera_game = nullptr;
	GameObject* selected_object = nullptr;
	GameObject* checkbox = nullptr;
	GameObject* canvas = nullptr;
	LineSegment ray;

	Quad quad_tree;

	FBO* scene_fbo = nullptr;
	FBO* game_fbo = nullptr;
	float2 guizmo_pos;

	std::string point = ".";
	std::string scene_name = "Scene01.scene";
	bool draw_grid = true;
	bool wireframe_on = false;
	bool fill_on = true;
	bool point_on = false;
	bool camera_culling = false;
	bool is_over_guizmo = false;
	bool is_over_scene = false;
	bool is_over_game = false;

	bool to_load_scene01 = false;

public:
	ImGuizmo::OPERATION guizmo_op      = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE      guizmo_mode    = ImGuizmo::WORLD;

	bool vsync = false;
	

};
