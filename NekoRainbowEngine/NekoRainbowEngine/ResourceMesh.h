#ifndef _RESOURCE_MESH_H_
#define _RESOURCE_MESH_H_

#include "Resource.h"
#include "MathGeoLib/include/Math/float3.h"

enum buffer_type
{
	BUFF_VERTICES,
	BUFF_UV,
	BUFF_NORMAL,
	BUFF_NORMAL_FACE,
	BUFF_INDEX,
	BUFF_TOTAL
};

class ResourceMesh: public Resource
{
public:
	ResourceMesh();
	ResourceMesh(Random id);
	~ResourceMesh();

	void GenerateBuffers();

public:
	//Index
	uint* index = nullptr;
	uint index_size;

	//Vertices
	float3* vertices = nullptr;
	uint vertices_size;

	//UV
	float2* UV_coord = nullptr;
	uint UV_size = 0;

	float3* normals_vertex = nullptr;
	uint norm_vertex_size = 0;

	float3* normals_face = nullptr;
	uint norm_face_size = 0;

	uint buffers[BUFF_TOTAL];
	std::string name;

};

#endif // !_RESOURCE_MESH_