#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>

class GameObject;

#define BUCKET 4

class QuadNode {
public:
	QuadNode(float3 min, float3 max);
	QuadNode(const AABB limits);
	~QuadNode() {};

	void Insert(GameObject* obj);
	void Remove(GameObject* obj) {};
	void DrawNode();

	void SubDivide();

public:
	AABB section;

	std::vector<GameObject*> node_objects;
	std::vector<QuadNode*> childrens;
	
};


class Quad
{
public:
	Quad() {};
	~Quad() {};

	void GenerateQuadtree(AABB section);
	void Insert(GameObject* obj) {};
	void Remove(GameObject* obj) {};

	void Draw();

private:
	QuadNode* root = nullptr;

	std::vector<GameObject*> quad_objects;
};

#endif // !_OCTREE_H_

