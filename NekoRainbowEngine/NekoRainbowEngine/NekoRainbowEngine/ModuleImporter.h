#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

#include "Primitive.h"

struct aiScene;
class GameObject;

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

private:

};