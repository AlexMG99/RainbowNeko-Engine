#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

//----------------- Mesh -----------------//
class Mesh {
public:
	Mesh() {};
	~Mesh() {};

	void GenerateMesh();
	void Render();

public:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

};

//----------------- FBX -----------------//

class FBX {
public:
	FBX() {};
	~FBX() {};

public:
	std::list<Mesh*> mesh_list;

};


class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadFile(const char* path);

private:
	std::list<FBX*> fbx_list;
};