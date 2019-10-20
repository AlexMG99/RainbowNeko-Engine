#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class GameObject;

enum component_type {
	COMPONENT_NONE = -1,

	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_SHAPE,
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
	uint shape_num = 0;

};

//-------------- Component Transform --------------
class ComponentTransform: public Component {
public:
	ComponentTransform(component_type comp_type, bool act, GameObject* obj) : Component(comp_type, act, obj) {};
	~ComponentTransform() {};

	bool Update() { return true; };

	void Enable() {};
	void Disable() {};

public:
	float position[3];
	float scale[3];
	Quat rotation;
};


#endif // !_COMPONENT_H_

