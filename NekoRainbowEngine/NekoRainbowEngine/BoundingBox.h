#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>

typedef unsigned int uint;

class ResourceMesh;
class GameObject;

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void GenerateBoundingBuffers();
	AABB CreateLocalAABB(ResourceMesh * mesh);
	AABB GetGlobalAABB(GameObject * obj);
	OBB GetOBB(GameObject * obj);

	void DrawBB();
	void UpdateBB(GameObject * obj);
	void CreateTransformAABB(GameObject * obj);

	void CleanVertices();

public:
	//AABB
	AABB local_AABB;
	AABB global_AABB;
	OBB global_OBB;

	//Vertex
	std::vector<float3> vertices_AABB;
	std::vector<float3> vertices_OBB;
	std::vector<uint> index_BB;

	//Show
	bool show_aabb = false;
	bool show_obb = false;

private:
	//Index
	uint id_vertexAABB = 0;
	uint id_vertexOBB = 0;
	uint id_indexBB = 0;


};

#endif // !_BOUNDING_BOX_H_
