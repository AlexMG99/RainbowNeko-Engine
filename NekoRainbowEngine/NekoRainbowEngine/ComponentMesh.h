#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include <string>
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "glmath.h"

#include <vector>

//-------------- Component Mesh --------------
class ComponentMesh : public Component {
public:
	ComponentMesh(component_type comp_type, bool act, GameObject * obj);
	~ComponentMesh();

	bool Update();

	void Enable() {};
	void Disable() {};

	void GenerateBuffers();

	//----- Bounding box -----//
	void GenerateBoundingBuffers();
	AABB CreateLocalAABB();
	AABB GetGlobalAABB();
	OBB GetOBB();

	void DrawBB();
	void UpdateBB();

	void RenderFill();
	void RenderWireframe();
	void RenderPoint();

public:
	//AABB
	AABB local_AABB;
	AABB global_AABB;
	OBB global_OBB;

	uint id_vertexAABB = 0;
	uint id_vertexOBB = 0;
	uint id_indexBB = 0;

	std::vector<float3> vertices_AABB;
	std::vector<float3> vertices_OBB;
	std::vector<uint> index_BB;

	//Mesh Properties
	uint id_index = 0;
	uint* index;
	uint index_size;

	uint id_vertex = 0;
	float3* vertices;
	uint vertices_size;

	//UVs
	float2* UV_coord = nullptr;
	uint image_id = -1;
	uint uv_id = 0;
	uint UV_size = 0;

	//Normals
	float3* normals = nullptr;
	std::vector<float3> normals_face;
	uint normal_id = -1;

	bool normal_show = false;
	bool normal_face_show = false;
	bool show_aabb = false;
	bool show_obb = false;
	bool camera_culling = false;

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

