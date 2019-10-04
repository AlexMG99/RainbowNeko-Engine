#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool CleanUp();


private:

};


//----------------- FBX -----------------//

class FBX {
public:
	FBX();
	~FBX();

	void LoadFile(const char* path);
private:
	std::list<Mesh*> mesh_list;
	
};

//----------------- Mesh -----------------//
class Mesh {
public:
	Mesh();
	~Mesh();

public:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_index = 0;
	uint* vertex = nullptr;

};