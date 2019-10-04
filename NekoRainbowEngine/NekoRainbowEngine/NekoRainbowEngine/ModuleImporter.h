#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

//----------------- Mesh -----------------//
class Mesh {
public:
	Mesh() {};
	~Mesh() {};

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
	std::list<Mesh> mesh_list;

};


class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool CleanUp();

	void LoadFile(const char* path);
private:
	FBX fbx_1;
};