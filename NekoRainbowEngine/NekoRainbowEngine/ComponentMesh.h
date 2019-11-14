#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include <string>
#include "glmath.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

#include <vector>

class Mesh;

//-------------- Component Mesh --------------
class ComponentMesh : public Component {
public:
	ComponentMesh(component_type comp_type, bool act, GameObject * obj);
	~ComponentMesh();

	bool Update();

	void Enable() {};
	void Disable() {};

	bool OnSave(Scene& scene) const;
	//bool OnLoad(Scene& scene) const;

	void AddMesh(Mesh* mesh);

	//----- Bounding box -----//
	void GenerateBoundingBuffers();
	AABB CreateLocalAABB();
	AABB GetGlobalAABB();
	AABB GetAABB();
	OBB GetOBB();

	//-------- Render--------//
	void RenderFill();
	void RenderWireframe();
	void RenderPoint();

	void DrawSelectedOutline();

public:
	Mesh* mesh = nullptr;

	uint image_id = -1;

	bool normal_show = false;
	bool normal_face_show = false;
	bool camera_culling = false;

	//Color Wireframe
	vec4 wireframe_color;
	vec4 vertex_color;
	vec4 outline_color;
	int point_size;
	int line_width;
	int outline_width;

	//Component Transform
	ComponentTransform* transform = nullptr;

private:
	AABB aabb;
	

};

class ComponentTexture : public Component {
public:
	ComponentTexture(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj) {};
	~ComponentTexture() {};

	void Enable() {};
	void Disable() {};

	void GenerateTexture();

public:
	uint width = 0, height = 0;
	uint image_id = 0;
	std::string path = "";
};

#endif // !COMPONENTMESH_H_

