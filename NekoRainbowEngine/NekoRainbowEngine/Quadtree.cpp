#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "Quadtree.h"

#include "GL/include/glew.h"

void Quad::GenerateQuadtree(AABB section)
{
	root = new QuadNode(section);

	root->SubDivide();
}

void QuadNode::SubDivide()
{
	float3 mid_point = float3(section.minPoint.x + (section.maxPoint.x - section.minPoint.x) * 0.5F,
		section.minPoint.y + section.maxPoint.y - section.minPoint.y,
		section.minPoint.z + (section.maxPoint.z - section.minPoint.z) * 0.5F);

	childrens.push_back(new QuadNode(section.minPoint, mid_point));
	childrens.push_back(new QuadNode(mid_point, section.maxPoint));

	childrens.push_back(new QuadNode(float3(min(mid_point.x, section.maxPoint.x), min(section.minPoint.y, section.maxPoint.y), min(mid_point.z, section.minPoint.z)), float3(max(mid_point.x, section.maxPoint.x), max(section.minPoint.y, section.maxPoint.y), max(mid_point.z, section.minPoint.z))));
	childrens.push_back(new QuadNode(float3(min(mid_point.x, section.minPoint.x), min(section.minPoint.y, section.maxPoint.y), min(mid_point.z, section.maxPoint.z)), float3(max(mid_point.x, section.minPoint.x), max(section.minPoint.y, section.maxPoint.y), max(mid_point.z, section.maxPoint.z))));

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

void QuadNode::Insert(GameObject * obj)
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

