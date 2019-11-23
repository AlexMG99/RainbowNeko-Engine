#ifndef _QUADTREE_H_
#define _QUADTREE_H_


#define BUCKET 2

#define CHILD 4

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Globals.h"
#include <vector>


class GameObject;
class QuadtreeNode;


class Quad
{
	friend class QuadNode;
public:
	Quad(const AABB& quad);
	~Quad();

	
	void Insert(GameObject* object);
	bool DeleteGObject(GameObject* obj);

	void Draw();


	void Clear();

private:
	QuadNode * root = nullptr;
	std::vector<GameObject*> node_objects;
};


class QuadNode
{
	friend class Quad;

public:
	QuadNode(const AABB& quad);
	QuadNode(Quad* quadtree, QuadNode* root, uint index);
	~QuadNode();

	bool AddToNode(GameObject* object);
	/*void Draw()*/;


	/*bool DeleteGOfromNode(const GameObject* object);*/

private:
	void SubdivideQuad();
	/*void RedistributeQuadTree();
	bool PutGameObjectToChilds(const GameObject* object);
	void RemoveChilds();*/
	/*void GetBucketChilds(std::vector<const GameObject*>& vector, bool wantSelf) const;*/

private:
	AABB quad_box;
	std::vector<QuadtreeNode> children;
	Quad* tree = nullptr;
	std::vector<const GameObject*> quad_objects;
};

#endif

