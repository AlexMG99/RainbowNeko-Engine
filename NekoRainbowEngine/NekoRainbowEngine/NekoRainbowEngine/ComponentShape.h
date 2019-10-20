#ifndef _COMPONENT_PAR_MESH_H_
#define _COMPONENT_PAR_MESH_H_

#include "Component.h"

typedef struct par_shapes_mesh_s par_shapes_mesh;

enum shape_type {
	SHAPE_NONE = -1,

	SHAPE_CUBE,
	SHAPE_SPHERE,
	SHAPE_CYLINDER,
	SHAPE_CONE,
	SHAPE_PLANE
};

class ComponentShape :public Component
{
public:
	ComponentShape(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj) {};
	~ComponentShape() {};

	bool Update();
	void CreateShape(shape_type type, uint sl, uint st);

private:
	par_shapes_mesh_s* shape;
	shape_type shape_t = SHAPE_NONE;
	uint slice = 0;
	uint stack = 0;
};
#endif // !_COMPONENT_PAR_MESH_H_

