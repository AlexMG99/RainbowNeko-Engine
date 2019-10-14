#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

GameObject::~GameObject()
{
	delete parent;
	parent = nullptr;

	for (auto it_obj = children.begin(); it_obj != children.end(); it_obj++) {
		delete (*it_obj);
		(*it_obj) = nullptr;
	}
	children.clear();

	for (auto it_comp = components.begin(); it_comp != components.end(); it_comp++) {
		delete (*it_comp);
		(*it_comp) = nullptr;
	}
	components.clear();
}

bool GameObject::Update()
{
	//Iterate Childrens
	for (auto it_obj = children.begin(); it_obj != children.end(); it_obj++) 
	{
		(*it_obj)->Update();
	}

	//Iterate Components
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		(*it_comp)->Update();
	}
	return true;
}

Component * GameObject::CreateComponent(component_type comp_type, bool act)
{
	Component* comp = nullptr;

	switch (comp_type)
	{
	case COMPONENT_TRANSFORM:
		comp = new ComponentTransform(comp_type, act, this);
		break;
	case COMPONENT_MESH:
		comp = new ComponentMesh(comp_type, act, this);
		break;
	case COMPONENT_TEXTURE:
		comp = new ComponentTexture(comp_type, act, this);
		break;
	case COMPONENT_NONE:
		break;
	}

	components.push_back(comp);

	return comp;
}

void GameObject::AddChildren(GameObject* obj)
{
	children.push_back(obj);
}

void GameObject::AddParent(GameObject * obj)
{
	parent = obj;
}

void GameObject::SetActive(bool act)
{
	active = act;
}
