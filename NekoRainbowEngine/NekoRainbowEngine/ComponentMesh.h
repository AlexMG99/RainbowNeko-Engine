#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include <string>
#include "glmath.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

#include <vector>

class ResourceMesh;
class ResourceTexture;

//-------------- Component Mesh --------------
class ComponentMesh : public Component {
public:
	ComponentMesh(component_type comp_type, bool act, GameObject * obj);
	~ComponentMesh();

	bool Update();

	void Enable() {};
	void Disable() {};

	bool OnSave(Scene& scene) const;
	bool OnLoad(Scene& scene);

	void AddMesh(ResourceMesh* mesh);

	//----- Bounding box -----//
	AABB CreateLocalAABB();

	//-------- Render--------//
	void RenderFill();
	void RenderWireframe();
	void RenderPoint();

	void DrawSelectedOutline();

public:
	ResourceMesh* mesh = nullptr;

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

};

class ComponentTexture : public Component {
public:
	ComponentTexture(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj) {};
	~ComponentTexture() {};

	void Enable() {};
	void Disable() {};

	bool OnSave(Scene& scene) const;
	bool OnLoad(Scene& scene);

	void AddTexture(ResourceTexture* text);

public:
	ResourceTexture* texture = nullptr;
};

#endif // !COMPONENTMESH_H_

