#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

struct aiScene;
struct aiNode;
class GameObject;
class vec3;

typedef struct par_shapes_mesh_s par_shapes_mesh;

enum shape_type {
	SHAPE_NONE = -1,

	SHAPE_CUBE,
	SHAPE_SPHERE,
	SHAPE_CYLINDER,
	SHAPE_CONE,
	SHAPE_PLANE
};

//----------------- ModuleImporter -----------------//
class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	bool CleanUp();

	void CreateShape(shape_type type, uint sl, uint st);

	bool ImportFBX(char* path_fbx, char* path_texture = "");
	bool ImportTexture(char* path_texture);

private:
	void RecursiveChild(const aiNode * node, char * path_fbx, const aiScene * scene, char * path_texture, GameObject* parent = nullptr);
	GameObject* CreateObject(const aiNode * node,  char * path_fbx, const aiScene * scene, char * path_texture);
	void CalculateNormalTriangle(ComponentMesh * m, vec3 triangle_p1, vec3 triangle_p2, vec3 triangle_p3);
};

void LogCallback(const char * text, char * data);
