#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "MathGeoLib/include/Geometry/AABB.h"

class GameObject;

enum QuadIndex {
	TOP_SW,
	TOP_SE,
	TOP_NW,
	TOP_NE,
	BOT_SW,
	BOT_SE,
	BOT_NW,
	BOT_NE
};

class QuadNode {
public:
	QuadNode(const AABB limits);
	~QuadNode() {};

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

public:
	AABB box;
	void Draw();
};


class Quad
{
public:
	Quad() {};
	~Quad() {};

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void Draw();

private:
	QuadNode* root = nullptr;
};

#endif // !_OCTREE_H_

