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

	bool Update();

	Component* CreateComponent(component_type comp_type, bool active = true);
	void AddChildren(GameObject* obj);
	void AddParent(GameObject* obj);
	void SetActive(bool act);
	std::string GetName() const;
	void SetName(const char* name_);


private:
	object_type type = OBJECT_NONE;
	std::string name = "Object Null";
	bool active = true;
	
	GameObject*					parent = nullptr;
	std::vector<Component*>		components;

public:
	std::vector<GameObject*>	children;
};

#endif // !_GAMEOBJECT_H_

