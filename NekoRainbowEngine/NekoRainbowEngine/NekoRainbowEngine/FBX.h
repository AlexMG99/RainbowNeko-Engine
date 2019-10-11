#ifndef _FBX_H_
#define _FBX_H_

#include "Globals.h"
#include <list>

#include "MathGeoLib/include/Math/float3.h"

//----------------- Mesh -----------------//
class Mesh {
public:
	Mesh() {};
	~Mesh();

	void GenerateMesh();
	void Render();

public:
	//Mesh Properties
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	//UVs
	float3* UV_coord = nullptr;
	uint image_id = -1;
	uint uv_id = 0;
	uint UV_num = 0;

	//Normals
	float3* normals;

};

//----------------- Texture -----------------//
class Texture {
public:
	Texture() {};
	~Texture() {};

	void GenerateTexture(uint& id);
	void Render();

public:
	uint image_id = -1;
	uint width = 0, height = 0;

};

//----------------- FBX -----------------//

class FBX {
public:
	FBX() {};
	~FBX();

	bool LoadTextures(Mesh* mesh, const char* path);

public:
	std::list<Mesh*> mesh_list;
	Texture* texture = nullptr;
};





#endif // !_FBX_H_

