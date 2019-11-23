#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "Quadtree.h"
#include "ComponentMesh.h"

#include "GL/include/glew.h"

Quad::~Quad()
{
	if (root != nullptr)
		RELEASE(root);
}

void Quad::GenerateQuadtree(AABB section)
{
	if (root) {
		RELEASE(root);
		quad_objects.clear();
	}

	root = new QuadNode(section);

}

void Quad::Insert(GameObject * obj)
{
	ComponentMesh* comp_mesh = obj->GetComponentMesh();
	if (obj->is_static && comp_mesh && comp_mesh->mesh)
	{
		if (!root)
		{
			GenerateQuadtree(comp_mesh->BB_mesh.GetGlobalAABB(obj));
			root->node_objects.push_back(obj);
		}
		else if (!IsGameobjectQuad(obj))
		{
			quad_objects.push_back(obj);
			root->Insert(obj, comp_mesh->BB_mesh.GetGlobalAABB(obj));
		}
	}

	for (auto it_child = obj->children.begin(); it_child != obj->children.end(); it_child++)
	{
		Insert(*it_child);
	}
}

void Quad::DeleteQuad()
{
	if (root)
	{
		root->DeleteNode();
		RELEASE(root);
	}

	for (auto it_obj = quad_objects.begin(); it_obj != quad_objects.end(); it_obj)
	{
		it_obj = quad_objects.erase(it_obj);
	}

	quad_objects.clear();

}

void QuadNode::SubDivide()
{
	float3 mid_point = float3(section.minPoint.x + (section.maxPoint.x - section.minPoint.x) * 0.5F,
		section.minPoint.y + section.maxPoint.y - section.minPoint.y,
		section.minPoint.z + (section.maxPoint.z - section.minPoint.z) * 0.5F);

	AddNode(section.minPoint, mid_point);
	AddNode(mid_point, section.maxPoint);

	AddNode(float3(min(mid_point.x, section.maxPoint.x), min(section.minPoint.y, section.maxPoint.y), min(mid_point.z, section.minPoint.z)), float3(max(mid_point.x, section.maxPoint.x), max(section.minPoint.y, section.maxPoint.y), max(mid_point.z, section.minPoint.z)));
	AddNode(float3(min(mid_point.x, section.minPoint.x), min(section.minPoint.y, section.maxPoint.y), min(mid_point.z, section.maxPoint.z)), float3(max(mid_point.x, section.minPoint.x), max(section.minPoint.y, section.maxPoint.y), max(mid_point.z, section.maxPoint.z)));

	for (auto it_obj = node_objects.begin(); it_obj != node_objects.end();)
	{
		bool erase = false;
		for (auto it_child = childrens.begin(); it_child != childrens.end(); it_child++)
		{
			ComponentMesh* comp_mesh = (*it_obj)->GetComponentMesh();
			if (comp_mesh && (*it_child)->section.Contains(comp_mesh->BB_mesh.GetGlobalAABB(*it_obj)))
			{
				(*it_child)->Insert(*it_obj, comp_mesh->BB_mesh.GetGlobalAABB(*it_obj));
				erase = true;
				break;
			}
		}
		if (erase)
			it_obj = node_objects.erase(it_obj);
		else
			it_obj++;
	}
}

void QuadNode::AddNode(const float3 min, const float3 max)
{
	QuadNode* quad_node = new QuadNode(min, max);
	quad_node->parent = this;
	childrens.push_back(quad_node);
}

bool QuadNode::AddToChildren(GameObject * obj, AABB aabb)
{
	bool ret = false;
	for (auto it_node = childrens.begin(); it_node != childrens.end(); it_node++)
	{
		if ((*it_node)->section.Contains(aabb))
		{
			(*it_node)->Insert(obj, aabb);
			ret = true;
			break;
		}
	}

	return ret;
}

void Quad::ReCalculate(GameObject * obj)
{
	std::vector<GameObject*> save_obj;
	AABB aabb;

	save_obj.push_back(obj);
	aabb = obj->GetComponentMesh()->BB_mesh.GetGlobalAABB(obj);

	root->SaveNodeObjects(save_obj, aabb);

	GenerateQuadtree(aabb);

	for (auto it_save = save_obj.begin(); it_save != save_obj.end(); it_save++)
	{
		Insert(*it_save);
	}
}

bool Quad::IsGameobjectQuad(GameObject * obj)
{
	bool ret = false;

	if (!quad_objects.empty()) {
		ret = std::find(quad_objects.begin(), quad_objects.end(), obj) == quad_objects.begin();
	}

	return ret;
}

void Quad::Draw()
{
	if(root)
		root->DrawNode();
}

QuadNode::QuadNode(float3 min, float3 max)
{
	section.minPoint = min;
	section.maxPoint = max;
}

QuadNode::QuadNode(const AABB limits)
{
	section.SetNegativeInfinity();
	section = limits;
}

void QuadNode::Insert(GameObject * obj, AABB aabb)
{
	if (section.Contains(aabb))
	{
		if (childrens.empty()) {
			if (node_objects.size() < BUCKET)
				node_objects.push_back(obj);
			else
			{
				SubDivide();
				if (!AddToChildren(obj, aabb))
				{
					EmptyNode();
					node_objects.push_back(obj);
				}
			}
		}
		else
		{
			if (!AddToChildren(obj, aabb))
				node_objects.push_back(obj);
		}
	}
	else
		App->viewport->quad_tree.ReCalculate(obj);
}

void QuadNode::DrawNode()
{
	glLineWidth(3.0f);

	glBegin(GL_LINES);

	glColor3f(1, 0, 0.81);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glEnd();

	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->DrawNode();
	}

	glLineWidth(1.0f);
	glColor3f(1, 1, 1);

}

void QuadNode::DeleteNode()
{
	for (auto it_node = node_objects.begin(); it_node != node_objects.end(); it_node)
	{
		it_node = node_objects.erase(it_node);
	}

	node_objects.clear();

	for (auto it_child = childrens.begin(); it_child != childrens.end(); it_child++)
	{
		(*it_child)->DeleteNode();
		RELEASE(*it_child);
	}

	childrens.clear();
}

void QuadNode::EmptyNode()
{
	for (auto it_child = childrens.begin(); it_child != childrens.end(); it_child++)
	{
		RELEASE(*it_child);
	}

	childrens.clear();
}

void QuadNode::SaveNodeObjects(std::vector<GameObject*>& save_vec, AABB & aabb)
{
	if (!node_objects.empty())
	{
		for (auto it_node = node_objects.begin(); it_node != node_objects.end(); it_node++)
		{
			save_vec.push_back(*it_node);
			AABB aux_aabb = (*it_node)->GetComponentMesh()->BB_mesh.GetGlobalAABB(*it_node);
			aabb.minPoint = float3(min(aabb.minPoint.x, aux_aabb.minPoint.x), min(aabb.minPoint.y, aux_aabb.minPoint.y), min(aabb.minPoint.z, aux_aabb.minPoint.z));
			aabb.maxPoint = float3(max(aabb.maxPoint.x, aux_aabb.maxPoint.x), max(aabb.maxPoint.y, aux_aabb.maxPoint.y), max(aabb.maxPoint.z, aux_aabb.maxPoint.z));
		}
	}

	node_objects.clear();

	for (auto it_child = childrens.begin(); it_child != childrens.end(); it_child++)
	{
		(*it_child)->SaveNodeObjects(save_vec, aabb);
	}
}

