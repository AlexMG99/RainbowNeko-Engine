#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

#include "Primitive.h"

struct aiScene;
struct aiNode;
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

	bool ImportFBX(char* path_fbx, char* path_texture = "");
	bool ImportTexture(char* path_texture);

private:
	void RecursiveChild(const aiNode * node, char * path_fbx, const aiScene * scene, char * path_texture);
	void CreateObject(const aiNode * node,  char * path_fbx, const aiScene * scene, char * path_texture);
};

void LogCallback(const char * text, char * data);
