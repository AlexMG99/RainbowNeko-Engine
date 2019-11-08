#include "Application.h"
#include "ComponentCamera.h"
#include "Parson/parson.h"

#include "MathGeoLib/include/Geometry/Frustum.h"

ComponentCamera::ComponentCamera(component_type comp_type, bool act, GameObject* obj): Component(comp_type, act, obj)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

void ComponentCamera::CreateFrustum()
{
	/*camera_frustum*/
}

float * ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix;
}

void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

update_status ComponentCamera::Load()
{
	//BROFILER_CATEGORY("Load_ModuleCamera3D", Profiler::Color::LightGray);

	//JSON_Object* obj = json_object(App->settings_doc);
	//JSON_Object* cam_obj = json_object_get_object(json_object_get_object(obj, "Application"), "Camera");

	//base_speed = json_object_get_number(cam_obj, "Speed");
	//Position = { (float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "X"),
	//	(float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "Y"),
	//	(float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "Z") };

	return UPDATE_CONTINUE;
}
