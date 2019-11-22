#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>

class GameObject;

#define BUCKET 4

enum QuadIndex {
	SW,
	SE,
	NW,
	NE
};

class QuadNode {
public:
	QuadNode(const AABB limits);
	~QuadNode() {};

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);
	void DrawNode();

public:
	AABB box;
	
};


class Quad
{
public:
	Quad() {};
	~Quad() {};

	void GenerateOctree(AABB section);
	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void Draw();

private:
	QuadNode* root = nullptr;

	std::vector<GameObject*> node_objects;
};

#endif // !_OCTREE_H_

