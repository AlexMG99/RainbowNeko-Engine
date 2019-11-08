#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"

GameObject::~GameObject()
{
	RELEASE_ARRAY_LIST(children);
	RELEASE_ARRAY_LIST(components);
}

bool GameObject::Update()
{
	//Iterate Childrens
	for (auto it_obj = children.begin(); it_obj != children.end(); it_obj++) 
	{
		if((*it_obj)->active)
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
	case COMPONENT_CAMERA:
		comp = new ComponentCamera(comp_type, act, this);
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

bool GameObject::HasChildren() const
{
	if (children.empty())
		return false;
	else
		return true;
}

void GameObject::AddParent(GameObject * obj)
{
	parent = obj;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject* par)
{
	if (par)
	{
		parent = par;
		parent->children.push_back(this);
	}
	else if(par != App->viewport->root_object)
	{
		parent = App->viewport->root_object;
		App->viewport->root_object->children.push_back(this);
	}
}

object_type GameObject::GetType()
{
	return type;
}

void GameObject::SetType(object_type ty_)
{
	type = ty_;
}

uint32 GameObject::GetId() const
{
	return id.GetNumber();
}

void GameObject::SetId()
{
	id.GenerateRandomInt();
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
	name = str.c_str();
}

float3 GameObject::GetScale(const float3 scale) const
{
	float3 scale_;

	if ((scale.x >= 100) && (scale.y >= 100) && (scale.z >= 100))
		scale_ = float3(scale.x * 0.01, scale.y * 0.01, scale.z*0.01);
	else if ((scale.x >= 1000) && (scale.y >= 1000) && (scale.z >= 1000))
		scale_ = float3(scale.x * 0.001, scale.y * 0.001, scale.z*0.001);
	else
		scale_ = scale;

	return scale_;
}

void GameObject::DeleteComponent(Component * comp)
{
	for (auto it_comp = components.begin(); it_comp != components.end();)
	{
		if ((*it_comp) == comp) {
			RELEASE(*it_comp);
			it_comp = components.erase(it_comp);
		}
		else
			++it_comp;

	}
}

