#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/Ray.h"
#include "MathGeoLib/include/Geometry/Triangle.h"

class ComponentCamera;
class GameObject;

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera(Application* app, bool start_enabled = true);
	~ModuleEditorCamera();

	bool Start();
	update_status Load();
	update_status Save() { return UPDATE_CONTINUE; };
	update_status Update(float dt);
	bool CleanUp();

	ComponentCamera* GetCamera() const;
	void SetCameraToCenter();

	void FocusObject(GameObject * obj);

	void MoveTo(const vec3 & Pos);
	void LookAt(const float3 &Spot);

	void ChangeCamera(ComponentCamera* camera);
	void SetSceneCamera();

private:
	void Move(float dt);
	void Move(float motion_x, float motion_y);
	void Orbit(float motion_x, float motion_y);
	void Zoom(float zoom);
	void CreatingRay(float norm_x, float norm_y);

public:
	float move_speed = 10.0f;
	float move_mouse_speed = 5.0f;
	float rot_speed = 10.0f;
	float zoom_speed = 150.0f;

	Ray my_ray;
	Ray local_ray;
	Triangle tri;

private:
	GameObject* obj_list;
	float3 looking_point;
	ComponentCamera* camera = nullptr;
	ComponentCamera* scene_camera = nullptr;
};