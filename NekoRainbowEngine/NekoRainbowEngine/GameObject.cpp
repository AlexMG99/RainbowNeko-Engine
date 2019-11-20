#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Scene.h"

#include "GL/include/glew.h"

GameObject::GameObject()
{
	local_AABB.SetNegativeInfinity();
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

	if (show_aabb || show_obb)
		DrawBB();

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

//--------------------- Bounding Box --------------------------//

void GameObject::GenerateBoundingBuffers()
{
	//Global AABB vertices
	glGenBuffers(1, &id_vertexAABB);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertexAABB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices_AABB.size(), &vertices_AABB[0], GL_STATIC_DRAW);

	//Global OBB vertices
	glGenBuffers(1, &id_vertexOBB);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertexOBB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices_OBB.size(), &vertices_OBB[0], GL_STATIC_DRAW);

	//Global BB vertices index
	glGenBuffers(1, &id_indexBB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexBB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* index_BB.size(), &index_BB[0], GL_STATIC_DRAW);
}

void GameObject::TransformBoundingBox()
{
	/*bbox.obb = bbox.aabb_local;
	bbox.obb.Transform(transform->GetGlobalMatrix());

	bbox.aabb_global.SetNegativeInfinity();
	bbox.aabb_global.Enclose(bbox.obb);

	bbox.min = bbox.aabb_local.minPoint;
	bbox.max = bbox.aabb_local.maxPoint;*/
}

void GameObject::DrawBB()
{
	glColor3f(125, 125, 0);
	glLineWidth(2.0);

	glEnableClientState(GL_VERTEX_ARRAY);

	//Draw Global AABB
	if (show_aabb)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_vertexAABB);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexBB);
		glDrawElements(GL_LINES, index_BB.size(), GL_UNSIGNED_INT, NULL);
	}

	//Draw OBB
	glColor3f(0, 200, 150);
	if (show_obb)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_vertexOBB);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexBB);
		glDrawElements(GL_LINES, index_BB.size(), GL_UNSIGNED_INT, NULL);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glLineWidth(1);
}

OBB GameObject::GetOBB()
{
	global_OBB = local_AABB;
	global_OBB.Transform(GetComponentTransform()->GetGlobalTransformMatrix());

	//Get Vertex and Index
	float3* aux_vertices = new float3[8];
	global_OBB.GetCornerPoints(aux_vertices);
	for (int i = 0; i < 8; i++)
	{
		vertices_OBB.push_back(aux_vertices[i]);
	}

	return global_OBB;
}

AABB GameObject::GetGlobalAABB()
{
	global_AABB.SetNegativeInfinity();
	global_AABB.Enclose(GetOBB());

	//Get Vertex and Index
	float3* aux_vertices = new float3[8];
	global_AABB.GetCornerPoints(aux_vertices);
	for (int i = 0; i < 8; i++)
	{
		vertices_AABB.push_back(aux_vertices[i]);
	}
	index_BB = { 0,1, 0,4, 4,5, 5,1,	//Front
	3,2, 2,0, 0,1, 1,3,
	7,6, 6,2, 2,3, 3,7,
	6,4, 2,0,
	7,5, 3,1 };

	return global_AABB;
}

void GameObject::UpdateBB()
{
	CleanVertices();

	glDeleteBuffers(1, &id_vertexAABB);
	glDeleteBuffers(1, &id_indexBB);
	glDeleteBuffers(1, &id_vertexOBB);

	GetGlobalAABB();

	if (parent && parent != App->viewport->root_object)
	{
		parent->CreateTransformAABB();
	}

	GenerateBoundingBuffers();
}

void GameObject::CreateTransformAABB()
{
	local_AABB.SetNegativeInfinity();

	for (auto it_child = children.rbegin(); it_child != children.rend(); it_child++)
	{
		local_AABB.Enclose((*it_child)->global_AABB);
	}

	CleanVertices();

	GetGlobalAABB();
	GenerateBoundingBuffers();

}

void GameObject::CleanVertices()
{
	for (int i = 0; i < vertices_AABB.size();)
	{
		vertices_AABB.pop_back();
	}
	vertices_AABB.clear();

	for (int i = 0; i < vertices_OBB.size();)
	{
		vertices_OBB.pop_back();
	}
	vertices_OBB.clear();
}


