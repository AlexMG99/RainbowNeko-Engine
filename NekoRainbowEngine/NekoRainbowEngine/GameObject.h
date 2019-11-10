#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <vector>

#include "Component.h"
#include "Random.h"

class ComponentMesh;
class ComponentTexture;
class ComponentCamera;

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
	ComponentCamera* GetComponentCamera();

	bool HasChildren() const;
	bool IsChild(GameObject* obj) const;
	bool IsDirectChild(GameObject * obj) const;
	void AddChild(GameObject* obj);
	void RemoveChild(GameObject* obj);

	void AddParent(GameObject* obj);
	GameObject* GetParent() const;
	void SetParent(GameObject * par);

	object_type GetType();
	void SetType(object_type ty_);

	uint32 GetId() const;
	void SetId();

	std::string GetName() const;
	void SetName(const char* name_);


	float3 GetScale(const float3 scale) const;

	void DeleteComponent(Component* comp);

private:
	object_type type = OBJECT_NONE;
	std::string name = "Object Null";
	
	Random	id;
	GameObject*					parent = nullptr;
	std::vector<Component*>		components;

public:
	std::vector<GameObject*>	children;
	bool selected = false;
	bool active = true;
};

#endif // !_GAMEOBJECT_H_

