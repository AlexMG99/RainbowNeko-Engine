#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_

#include "Component.h"
#include "Globals.h"
#include "glmath.h"

class ComponentCamera: public Component
{
public:
	ComponentCamera(component_type comp_type, bool act, GameObject* obj);
	~ComponentCamera() {};

	void CreateFrustum();

	float* GetViewMatrix();
	void CalculateViewMatrix();

	update_status Load();
	update_status Save() { return UPDATE_CONTINUE; };

public:
	vec3 X, Y, Z, Position, Reference;
	float base_speed = 0.01f;

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
	//Frustum camera_frustum;
};

#endif // !_COMPONENT_CAMERA_H_

