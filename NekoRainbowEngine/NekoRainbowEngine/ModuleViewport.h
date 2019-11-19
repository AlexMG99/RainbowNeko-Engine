#pragma once
#include "Module.h"
#include "Globals.h"
#include "par/par_shapes.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "RayCast.h"
#include "imGuizmo/ImGuizmo.h"


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

	//----Intersections----//
	//bool CompareRayCast(RayCast& a, RayCast & b);
	GameObject* MyRayCastIntersection(LineSegment* ray, RayCast& hit);

	void BoxIntersection(GameObject* obj, LineSegment* ray, std::vector<RayCast>& scene_obj);
	GameObject* TriangleTest(LineSegment& ray, GameObject* obj );

	

	bool LoadScene(Scene* scn);
	bool LoadGameObject(Scene scn);

	bool SaveScene();
	bool SaveGameObject(Scene go_scene, GameObject* obj, int* i);

	void ReorganizeHierarchy();

	bool ResetScene();

	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr, float3 position = { 0.0f,0.0f,0.0f } , float3 scale = { 1.0f,1.0f,1.0f }, Quat rotation = Quat::identity);
	
	void DeleteGameObject();

public:
	void DrawGrid(uint separation, uint lines);
	void GuizControls();
	void GuizLogic();
	

public:
	Scene* scene = nullptr;
	GameObject* root_object = nullptr;
	GameObject* camera_test = nullptr;
	GameObject* selected_object = nullptr;
	LineSegment ray;


	bool draw_grid = true;
	bool wireframe_on = false;
	bool fill_on = true;
	bool point_on = false;

private:
	ImGuizmo::OPERATION guizmo_op      = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE      guizmo_mode    = ImGuizmo::WORLD;
	

};
