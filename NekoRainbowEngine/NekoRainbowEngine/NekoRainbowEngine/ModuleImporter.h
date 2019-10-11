#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

#include "MathGeoLib/include/Math/float3.h"
#include "Devil/include/il.h"



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
	uint image_id = 0;

	//Normals
	float3* normals;

};

//----------------- Texture -----------------//
class Texture {
public:
	Texture() {};
	~Texture() {};

	void GenerateTexture();
	void Render();

public:
	uint image_id = 0;
	uint width, height;

};

//----------------- FBX -----------------//

class FBX {
public:
	FBX() {};
	~FBX();

	bool LoadTextures(const char* path);

public:
	std::list<Mesh*> mesh_list;
	Texture* texture = nullptr;
};

class Cube {
public:
	Cube() {};
	~Cube() {};

	void GenerateMesh();
	void Render();

public:
	par_shapes_mesh_s* cube_mesh = nullptr;

private:
	uint my_id = 0;
	uint my_indices = 0;
};

struct aiScene;

//----------------- ModuleImporter -----------------//
class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadFile(const char* path);
	bool LoadMesh(const aiScene* scene, FBX*& aux_fbx, const char*& path);
	Cube* CreateCube(int x, int y, int z);

	std::list<Cube*> GetCubeList() const;

private:
	std::list<FBX*> fbx_list;
public:
	std::list<Cube*> cube_list;
};