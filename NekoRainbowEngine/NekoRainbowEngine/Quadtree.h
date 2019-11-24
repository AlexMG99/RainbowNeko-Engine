#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>

class GameObject;

#define BUCKET 1
#define MAX_DIVISIONS 3

class QuadNode {
public:
	QuadNode(float3 min, float3 max);
	QuadNode(const AABB limits);
	~QuadNode() {};

	void Insert(GameObject* obj, AABB aabb);
	void Remove(GameObject* obj) {};
	void DrawNode();
	void DeleteNode();

	void SaveNodeObjects(std::vector<GameObject*>& save_vec, AABB& aabb);

private:
	void SubDivide();
	void AddNode(const float3 min, const float3 max);
	void EmptyNode();

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
	void DeleteQuad();

	void ReCalculate(GameObject* obj);
	bool IsGameobjectQuad(GameObject* obj);

	void Draw();

	uint GetDivisions() const;
	void AddDivision();

private:
	QuadNode* root = nullptr;

	std::vector<GameObject*> quad_objects;
	uint divisions = 0;
};

#endif // !_OCTREE_H_

