#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

#include "FBX.h"
#include "Primitive.h"

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

	bool ImportFBX(const char* path_fbx, char* path_texture = "");
	bool LoadMesh(const aiScene* scene, FBX*& aux_fbx, const char*& path, const char* path_tex);

private:
	std::list<FBX*> fbx_list;

};