#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject;

enum component_type {
	COMPONENT_NONE = -1,

	COMPONENT_TRANSFORMATION,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_LIGHT
};

//-------------- Component --------------

class Component {
public:
	Component() {};
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


#endif // !_COMPONENT_H_

