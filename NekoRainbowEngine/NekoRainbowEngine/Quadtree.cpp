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
	}

	root = new QuadNode(section);

}

void Quad::Insert(GameObject * obj)
{
	ComponentMesh* comp_mesh = obj->GetComponentMesh();
	if (comp_mesh && comp_mesh->mesh)
	{
		if (!root)
		{
			GenerateQuadtree(comp_mesh->BB_mesh.GetGlobalAABB(obj));
		}
		else if (!IsGameobjectQuad(obj))
		{
			quad_objects.push_back(obj);
			root->Insert(obj, comp_mesh->BB_mesh.GetGlobalAABB(obj));
		}
		
	}
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

	if (node_objects.size() > BUCKET)
	{

	}
}

void QuadNode::AddObjectNode(GameObject * obj)
{

}

void QuadNode::AddNode(const float3 min, const float3 max)
{
	QuadNode* quad_node = new QuadNode(min, max);
	quad_node->parent = this;
	childrens.push_back(quad_node);
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

}

void QuadNode::DrawNode()
{
	glBegin(GL_LINES);

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

}

