#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"

//-------------- Component Transform --------------
class ComponentMesh : public Component {
public:
	ComponentMesh(component_type comp_type, bool act, GameObject * obj) : Component(type, active, my_go) {};
	~ComponentMesh() {};

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
	float3* normals;

};

#endif // !COMPONENTMESH_H_

