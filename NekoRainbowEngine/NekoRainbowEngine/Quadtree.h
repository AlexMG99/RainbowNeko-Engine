#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>

class GameObject;

#define BUCKET 1

class QuadNode {
public:
	QuadNode(float3 min, float3 max);
	QuadNode(const AABB limits);
	~QuadNode() {};

	void Insert(GameObject* obj, AABB aabb);
	void Remove(GameObject* obj) {};
	void DrawNode();

	void SaveNodeObjects(std::vector<GameObject*>& save_vec, AABB& aabb);

private:
	void SubDivide();
	void AddObjectNode(GameObject* obj);
	void AddNode(const float3 min, const float3 max);

	bool AddToChildren(GameObject* obj, AABB aabb);

public:
	AABB section;

	QuadNode* parent = nullptr;
	std::vector<GameObject*> node_objects;
	std::vector<QuadNode*> childrens;
	
};


class Quad
{
public:
	Quad() {};
	~Quad();

	void GenerateQuadtree(AABB section);
	void Insert(GameObject* obj);
	void Remove(GameObject* obj) {};

	void ReCalculate(GameObject* obj);

	bool IsGameobjectQuad(GameObject* obj);

	void Draw();

private:
	QuadNode* root = nullptr;

	std::vector<GameObject*> quad_objects;
};

#endif // !_OCTREE_H_

