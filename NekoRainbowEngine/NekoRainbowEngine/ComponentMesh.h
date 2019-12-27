#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include <string>
#include "glmath.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "BoundingBox.h"
#include "RayCast.h"

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

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

	void AddMesh(ResourceMesh* mesh);
	void ChangeColor(vec4 color);
	bool Fade(float dt);

	//-------- Render--------//
	void RenderFill();
	void RenderWireframe();
	void RenderPoint();

	bool DrawSelectedOutline();

private:
	uint id_vertexAABB = 0;
	uint id_vertexOBB = 0;
	uint id_indexBB = 0;

public:
	ResourceMesh* mesh = nullptr;
	BoundingBox BB_mesh;

	uint image_id = -1;

	bool normal_show = false;
	bool normal_face_show = false;
	bool camera_culling = false;

	//Color Wireframe
	vec4 fill_color;
	vec4 wireframe_color;
	vec4 vertex_color;
	vec4 outline_color;
	int point_size = 1.0;
	int line_width = 1.0;
	int outline_width = 1.0;

	//Component Transform
	ComponentTransform* transform = nullptr;

};

class ComponentTexture : public Component {
public:
	ComponentTexture(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj) {};
	~ComponentTexture();

	void Enable() {};
	void Disable() {};

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

	void AddTexture(ResourceTexture* text);

public:
	ResourceTexture* texture = nullptr;
};

#endif // !COMPONENTMESH_H_

