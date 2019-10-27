#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include <string>
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "glmath.h"


//-------------- Component Mesh --------------
class ComponentMesh : public Component {
public:
	ComponentMesh(component_type comp_type, bool act, GameObject * obj);
	~ComponentMesh();

	bool Update();

	void Enable() {};
	void Disable() {};

	void GenerateMesh();

	void RenderFill();
	void RenderWireframe();
	void RenderPoint();

public:
	//AABB
	AABB local_AABB;
	AABB global_AABB;
	OBB global_OBB;

	//Mesh Properties
	uint id_index = 0;
	std::vector<uint> index;

	uint id_vertex = 0;
	std::vector<float3> vertices;

	//UVs
	float* UV_coord = nullptr;
	uint image_id = -1;
	uint uv_id = 0;
	uint UV_num = 0;

	//Normals
	float3* normals = nullptr;
	std::vector<float3> normals_face;
	uint normal_id = -1;

	bool par_shape = false;
	bool normal_show = false;
	bool normal_face_show = false;

	//Color Wireframe
	vec4 wireframe_color;
	vec4 vertex_color;
	int point_size;
	int line_width;

	//Component Transform
	ComponentTransform* transform = nullptr;

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

