#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentShape.h"
#include "ComponentMesh.h"

#include "par/par_shapes.h"
#include "MathGeoLib/include/Math/Quat.h"

void ComponentShape::CreateShape(shape_type type, uint sl, uint st)
{
	//Initialize variables
	slice = sl;
	stack = st;
	shape_t = type;

	//Create shape
	switch (type)
	{
	case SHAPE_CUBE:
		shape = par_shapes_create_cube();
		break;
	case SHAPE_SPHERE:
		shape = par_shapes_create_parametric_sphere(sl, st);
		break;
	case SHAPE_CYLINDER:
		shape = par_shapes_create_cylinder(sl, st);
		break;
	case SHAPE_CONE:
		shape = par_shapes_create_cone(sl, st);
		break;
	case SHAPE_PLANE:
		shape = par_shapes_create_plane(sl, st);
		break;
	default:
		LOG("Shape type incorrect or inexistent!");
		break;
	}

	//Create Component Transform
	ComponentTransform* trans = (ComponentTransform*)my_go->CreateComponent(COMPONENT_TRANSFORM);
	trans->position[0] = shape->points[0];
	trans->position[1] = shape->points[1];
	trans->position[2] = shape->points[2];

	par_shapes_scale(shape, 1.0f, 1.0f, 1.0f);
	trans->scale[0] = 1;
	trans->scale[1] = 1;
	trans->scale[2] = 1;

	trans->rotation.x = 0;
	trans->rotation.y = 0;
	trans->rotation.z = 0;

	//Create Component Mesh
	ComponentMesh* mesh = (ComponentMesh*)my_go->CreateComponent(COMPONENT_MESH);
	mesh->vertices = shape->points;
	mesh->num_vertices = shape->npoints;
	mesh->index = (uint*)shape->triangles;
	mesh->num_index = shape->ntriangles * 3;
	mesh->par_shape = true;
	mesh->UV_coord = shape->tcoords;
	mesh->UV_num = 2;

	mesh->GenerateMesh();

	//Create Component Texture
	ComponentTexture* tex = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);

	my_go->SetName(std::string("GameObject " + std::to_string(App->viewport->shape_num)).c_str());
	App->viewport->shape_num++;
}

std::string ComponentShape::GetTypeString()
{
	std::string tmp_str = "No Object!";

	switch (shape_t)
	{
	case SHAPE_CUBE:
		tmp_str = "Cube";
		break;
	case SHAPE_SPHERE:
		tmp_str = "Sphere";
		break;
	case SHAPE_CYLINDER:
		tmp_str = "Cylinder";
		break;
	case SHAPE_CONE:
		tmp_str = "Cone";
		break;
	case SHAPE_PLANE:
		tmp_str = "Plane";
		break;
	default:
		break;
	}

	return tmp_str;
}
