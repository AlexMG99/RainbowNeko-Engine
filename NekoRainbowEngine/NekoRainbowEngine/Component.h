#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/Quat.h"

class GameObject;

enum component_type {
	COMPONENT_NONE = -1,

	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_TEXTURE,
	COMPONENT_LIGHT
};

//-------------- Component --------------

class Component {
public:
	Component(component_type comp_type, bool act, GameObject* obj);
	~Component();

	virtual bool Update() { return true; };

	virtual void Enable() {};
	virtual void Disable() {};

public:
	component_type type = COMPONENT_NONE;
	bool active = true;
	GameObject* my_go = nullptr;
};

//-------------- Component Transform --------------
class ComponentTransform: public Component {
public:
	ComponentTransform(component_type comp_type, bool act, GameObject* obj);
	~ComponentTransform() {};

	bool Update() { return true; };

	void Enable() {};
	void Disable() {};

	float4x4 GetGlobalTransformMatrix();

public:
	//Global
	float4x4 global_matrix;
	float3 position;
	float3 scale;
	Quat rotation;

	//Local
	float4x4 local_matrix;
	float3 local_position;
	float3 local_scale;
	float3 local_rotation_euler;
	Quat local_rotation;
};


#endif // !_COMPONENT_H_

