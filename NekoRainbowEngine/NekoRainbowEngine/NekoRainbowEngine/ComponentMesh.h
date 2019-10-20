#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include <string>

//-------------- Component Mesh --------------
class ComponentMesh : public Component {
public:
	ComponentMesh(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj) {};
	~ComponentMesh();

	bool Update();

	void Enable() {};
	void Disable() {};

	void GenerateMesh();
	void Render();

public:
	//Mesh Properties
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	//UVs
	float* UV_coord = nullptr;
	uint image_id = -1;
	uint uv_id = 0;
	uint UV_num = 0;

	//Normals
	float3* normals = nullptr;

	bool par_shape = false;

};

class ComponentTexture : public Component {
public:
	ComponentTexture(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj) {};
	~ComponentTexture() {};

	void Enable() {};
	void Disable() {};

	void LoadTexture(const char* path);
	void GenerateTexture();

public:
	uint width = 0, height = 0;
	uint image_id = 0;
	std::string path = "";
};

#endif // !COMPONENTMESH_H_

