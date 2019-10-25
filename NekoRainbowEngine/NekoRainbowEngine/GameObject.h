#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <vector>

#include "Component.h"
#include "ComponentMesh.h"

enum object_type {
	OBJECT_NONE = -1,

	OBJECT_FBX,
	OBJECT_PARSHAPE
};

class GameObject {
public:
	GameObject() {};
	~GameObject();

	bool Update();

	Component* CreateComponent(component_type comp_type, bool active = true);

	ComponentTransform* GetComponentTransform();
	ComponentMesh* GetComponentMesh();
	ComponentTexture* GetComponentTexture();

	void AddChildren(GameObject* obj);
	bool HasChildren() const;
	void AddParent(GameObject* obj);
	GameObject* GetParent() const;
	void SetParent(GameObject * par);

	object_type GetType();
	void SetType(object_type ty_);

	std::string GetName() const;
	void SetName(const char* name_);

	float3 GetScale(const float3 scale) const;


private:
	object_type type = OBJECT_NONE;
	std::string name = "Object Null";
	
	GameObject*					parent = nullptr;
	std::vector<Component*>		components;

public:
	std::vector<GameObject*>	children;
	bool selected = false;
	bool active = true;
};

#endif // !_GAMEOBJECT_H_

