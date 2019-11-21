#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <vector>

#include "Component.h"

enum object_type {
	OBJECT_NONE = -1,

	OBJECT_FBX,
	OBJECT_CUBE,
	OBJECT_SPHERE
};

class GameObject {
public:
	GameObject() {};
	~GameObject();

	bool Update() { return true; };

	Component* CreateComponent() {};

private:
	object_type type = OBJECT_NONE;
	std::string name = "Object Null";
	
	GameObject*					parent = nullptr;
	std::vector<GameObject*>	children;
	std::vector<Component*>		components;

};

#endif // !_GAMEOBJECT_H_

