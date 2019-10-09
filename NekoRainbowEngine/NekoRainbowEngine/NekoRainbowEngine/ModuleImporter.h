#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

#include "Assimp/include/vector3.h"

//----------------- Mesh -----------------//
class Mesh {
public:
	Mesh() {};
	~Mesh();

	void GenerateMesh();
	void Render();

public:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	aiVector3D* normals;

};

//----------------- FBX -----------------//

class FBX {
public:
	FBX() {};
	~FBX();

public:
	std::list<Mesh*> mesh_list;

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

//----------------- ModuleImporter -----------------//
class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadFile(const char* path);
	Cube* CreateCube(int x, int y, int z);

	std::list<Cube*> GetCubeList() const;

private:
	std::list<FBX*> fbx_list;
public:
	std::list<Cube*> cube_list;
};