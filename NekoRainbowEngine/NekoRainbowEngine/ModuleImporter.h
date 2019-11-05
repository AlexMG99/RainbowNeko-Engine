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
	SHAPE_PLANE,
	SHAPE_TORUS,
	SHAPE_OCTAHEDRON
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

	bool ImportFBX(char* path_fbx);
	bool ImportTexture(char* path_texture);

	bool Import(const char* file, const char* path, std::string& output_file);

private:
	void LoadNode(const aiNode * node, const aiScene * scene, char * path_fbx, GameObject* parent = nullptr);
	void CalculateNormalTriangle(ComponentMesh * m, vec3 triangle_p1, vec3 triangle_p2, vec3 triangle_p3);
};

void LogCallback(const char * text, char * data);
