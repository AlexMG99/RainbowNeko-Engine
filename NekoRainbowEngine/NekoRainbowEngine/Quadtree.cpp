#include "Quadtree.h"
#include "GameObject.h"

Quad::Quad(const AABB& quad)
{
	root = new QuadNode(quad);
	root->tree = this;
}

Quad::~Quad()
{
	RELEASE(root);
}

void Quad::Insert(GameObject * object)
{
	if (root->AddToNode(object) == false)
		node_objects.push_back(object); //the game object is outside the quadtree
}




QuadNode::QuadNode(const AABB& quad)
{
	quad_box = quad;
}

QuadNode::QuadNode(Quad * quadtree, QuadNode * root, uint index)
{
	float3 minPoint, maxPoint;

	minPoint.x = (index / 2) == 1 ? root->quad_box.minPoint.x : (root->quad_box.maxPoint.x + root->quad_box.minPoint.x) / 2;
	maxPoint.x = (index / 2) == 1 ? (root->quad_box.maxPoint.x + root->quad_box.minPoint.x) / 2 : root->quad_box.maxPoint.x;

	minPoint.y = root->quad_box.minPoint.y;
	maxPoint.y = root->quad_box.maxPoint.y;

	minPoint.z = index % 2 == 0 ? root->quad_box.minPoint.z : (root->quad_box.maxPoint.z + root->quad_box.minPoint.z) / 2;
	maxPoint.z = index % 2 == 0 ? (root->quad_box.maxPoint.z + root->quad_box.minPoint.z) / 2 : root->quad_box.maxPoint.z;
}

QuadNode::~QuadNode()
{
}

bool QuadNode::AddToNode( GameObject * object)
{
	if (quad_box.Intersects(object->bounding.global_AABB))
	{
		if (children.empty()) //No childs in the box?
		{
			quad_objects.push_back(object);
			if (quad_objects.size() > BUCKET)
			{
				SubdivideQuad();
				/*RedistributeQuadTree();*/
			}
		}
		else
		{
			/*if (!PutGameObjectToChilds(gameObject))
				bucket.push_back(gameObject);*/
		}

		return true;
	}

	else
		return false;
}

void QuadNode::SubdivideQuad()
{
	if (!children.empty()) {
		LOG("no spliting");
	}
	else
	{
		for (int i = 0; i < CHILD; i++)
		{
			/*children.push_back(QuadNode(tree, this, i));*/ //generate a node and subdivide it
		}
	}

}

