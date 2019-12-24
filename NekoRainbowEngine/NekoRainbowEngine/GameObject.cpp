#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentCanvas.h"
#include "ComponentButton.h"
#include "ComponentLabel.h"
#include "Scene.h"

#include "GL/include/glew.h"

GameObject::GameObject()
{
	
}

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
	case COMPONENT_CANVAS:
		comp = new ComponentCanvas(comp_type, act, this, 66, 80);
		break;
	case COMPONENT_NONE:
		break;
	}

	if(comp)
		components.push_back(comp);

	return comp;
}

ComponentCamera* GameObject::CreateComponentCamera(float nP, float fP, float FOV)
{
	ComponentCamera* comp = new ComponentCamera(COMPONENT_CAMERA, true, this, nP, fP, FOV);
	if (comp)
		components.push_back(comp);
	return comp;
}

ComponentUI * GameObject::CreateComponentUI(UI_type ui_type, uint width, uint height, ComponentCanvas* canvas, bool active)
{
	ComponentUI* comp = nullptr;

	switch (ui_type)
	{
	case UI_Button:
		comp = new ComponentButton(COMPONENT_UI, active, this, ui_type, width, height, canvas);
		break;
	case UI_Label:
		comp = new ComponentLabel(COMPONENT_UI, active, this, ui_type, width, height, canvas);
		break;
	case UI_InpuText:

		break;
	case UI_Checkbox:

		break;
	}

	if (comp)
		components.push_back(comp);

	return comp;
}

bool GameObject::SaveComponents(Scene scene)
{
	for (int i = 0; i < components.size(); i++)
	{
		components.at(i)->OnSave(scene, i);
	}
	return true;
}

bool GameObject::LoadComponents(Scene scene)
{
	Scene comp_scene = scene.GetArray("Components");

	for (int i = 1; i < COMPONENT_TOTAL; i++)
	{
		if (comp_scene.IsArraySection(i))
			CreateComponent((component_type)(comp_scene.GetSectionArray(i).GetInt("Type")));
	}
	
	for (int i = 0; i< components.size(); i++)
	{
		components.at(i)->OnLoad(comp_scene, i);
	}
	return true;
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

ComponentCamera* GameObject::GetComponentCamera()
{
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		if ((*it_comp)->type == COMPONENT_CAMERA)
			return (ComponentCamera*)(*it_comp);
	}

	return nullptr;
}

ComponentCanvas * GameObject::GetComponentCanvas()
{
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		if ((*it_comp)->type == COMPONENT_CANVAS)
			return (ComponentCanvas*)(*it_comp);
	}

	return nullptr;
}

ComponentUI * GameObject::GetComponentUI()
{
	for (auto it_comp = components.begin(); it_comp != components.end(); ++it_comp)
	{
		if ((*it_comp)->type == COMPONENT_UI)
			return (ComponentUI*)(*it_comp);
	}

	return nullptr;
}

bool GameObject::HasChildren() const
{
	if (children.empty())
		return false;
	else
		return true;
}

bool GameObject::IsChild(GameObject * obj) const
{
	bool ret = false;
	for (auto it_child = children.begin(); it_child != children.end(); it_child++)
	{
		if ((*it_child) == obj)
			return true;

		ret = (*it_child)->IsChild(obj);
	}
	return ret;
}

bool GameObject::IsDirectChild(GameObject * obj) const
{
	for (auto it_child = children.begin(); it_child != children.end(); it_child++)
	{
		if ((*it_child) == obj)
			return true;
	}
	return false;
}

void GameObject::AddChild(GameObject * obj)
{
	obj->SetParent(this);
}

void GameObject::RemoveChild(GameObject * obj)
{
	for (auto it_child = children.begin(); it_child != children.end();)
	{
		if ((*it_child) == obj)
		{
			it_child = children.erase(it_child);
		}
		else
			it_child++;
			
	}
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

bool GameObject::IsParentID(uint32 id)
{
	return (this->id.GetNumber() == id);
}

GameObject* GameObject::GetIteratorChild(uint32 id)
{
	for (auto it_child = children.begin(); it_child != children.end();it_child++)
	{
		if ((*it_child)->IsParentID(id))
			return *it_child;

		return (*it_child)->GetIteratorChild(id);
	}

	return nullptr;
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

void GameObject::SetId(double id)
{
	this->id.SetNumber(id);
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

void GameObject::SetStatic(const bool stc)
{
	is_static = stc;
	for (auto it_child = children.begin(); it_child != children.end(); it_child++)
	{
		(*it_child)->SetStatic(stc);
	}
}

void GameObject::SetSelected(bool select)
{
	selected = select;
	for (auto it_child = children.begin(); it_child != children.end(); it_child++)
	{
		(*it_child)->SetSelected(select);
	}
}


float3 GameObject::CorrectScale(const float3 scale) const
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
