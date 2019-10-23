#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "Component.h"

GameObject::~GameObject()
{
	for (auto it_obj = children.begin(); it_obj != children.end();) {
		RELEASE(*it_obj);
		it_obj = children.erase(it_obj);
	}
	children.clear();

	for (auto it_comp = components.begin(); it_comp != components.end();) {
		RELEASE(*it_comp);
		it_comp = components.erase(it_comp);
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
		if((*it_comp)->active)
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

ComponentTransform * GameObject::GetComponentTransform()
{
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		if((*it_comp)->type == COMPONENT_TRANSFORM)
			return (ComponentTransform*)(*it_comp);
	}

	return nullptr;
}

ComponentMesh * GameObject::GetComponentMesh()
{
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		if ((*it_comp)->type == COMPONENT_MESH)
			return (ComponentMesh*)(*it_comp);
	}

	return nullptr;
}

ComponentTexture* GameObject::GetComponentTexture()
{
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		if ((*it_comp)->type == COMPONENT_TEXTURE)
			return (ComponentTexture*)(*it_comp);
	}

	return nullptr;
}

void GameObject::AddChildren(GameObject* obj)
{
	children.push_back(obj);
}

void GameObject::AddParent(GameObject * obj)
{
	parent = obj;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetActive(bool act)
{
	active = act;
}

object_type GameObject::GetType()
{
	return type;
}

void GameObject::SetType(object_type ty_)
{
	type = ty_;
}

std::string GameObject::GetName() const
{
	std::size_t pos = name.find_last_of("/");
	std::string str = name.substr(pos + 1);
	return str;
}

void GameObject::SetName(const char * name_)
{
	std::string temp_str = name_;
	std::size_t pos = temp_str.find_last_of("/");
	if(pos >1000)
		pos = temp_str.find_last_of("\\");
	std::string str = temp_str.substr(pos + 1);
	name = str;
}

