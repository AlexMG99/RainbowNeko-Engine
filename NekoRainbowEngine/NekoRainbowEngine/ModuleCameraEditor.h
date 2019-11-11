#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"

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
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void SetCameraToCenter();

	void FocusObject(GameObject * obj);

	void MoveTo(const vec3 & Pos);

	void LookAt(float dx, float dy);
	void LookAt(const float3 &Spot);

private:
	void Move(float dt);
	void Move(float motion_x, float motion_y);
	void Orbit(float motion_x, float motion_y);
	void Zoom(float zoom);

public:
	float move_speed = 10.0f;
	float rot_speed = 0.1f;
	float zoom_speed = 150.0f;

private:
	float3 looking_point;
	ComponentCamera* camera = nullptr;
};