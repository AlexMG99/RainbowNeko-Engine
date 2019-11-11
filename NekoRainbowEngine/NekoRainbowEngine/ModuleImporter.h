#pragma once

#include "Module.h"
#include "Globals.h"
#include <list>

struct aiScene;
struct aiNode;
class GameObject;
class SceneImporter;
class MeshImporter;
class TextureImporter;
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
	bool CleanUp();

	void CreateShape(shape_type type, uint sl, uint st);

	bool ImportFile(const char* path_fbx);

private:
	SceneImporter* scene;
	MeshImporter* mesh;
	TextureImporter* texture;
};
