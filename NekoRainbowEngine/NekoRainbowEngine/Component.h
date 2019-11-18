#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class Scene;

enum component_type {
	COMPONENT_NONE = -1,

	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_TEXTURE,
	COMPONENT_LIGHT,
	COMPONENT_CAMERA,
	COMPONENT_TOTAL
};

//-------------- Component --------------

class Component {
public:
	Component(component_type comp_type, bool act, GameObject* obj);
	~Component();

	virtual bool Update() { return true; };

	virtual void Enable() {};
	virtual void Disable() {};

	virtual bool OnSave(Scene& scene) const { return true; };
	virtual bool OnLoad(Scene& scene) { return true; };

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

	bool ItIntersect(LineSegment ray);

	bool OnSave(Scene& scene) const;
	bool OnLoad(Scene& scene);

	float4x4 GetGlobalTransformMatrix();
	void RecalculateTransformMatrix();

	void SetLocalTransform(float4x4 & t_matrix);
	void CalculateGlobalAxis();

	void UpdateComponents();

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


	float4x4 global_transformation = float4x4::identity;
	float4x4 local_transformation = float4x4::identity;


	GameObject* game_item = nullptr;

	//Axis
	float3 X, Y, Z;

	
};


#endif // !_COMPONENT_H_

