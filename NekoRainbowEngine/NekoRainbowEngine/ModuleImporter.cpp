#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "PanelConsole.h"
#include "glmath.h"
#include <list>
#include <string>

#include"Brofiler/Brofiler.h"

#include "SceneImporter.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

//----------------- ModuleImporter -----------------//

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module (app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	BROFILER_CATEGORY("Init_ModuleImporter", Profiler::Color::Crimson);

	scene = new SceneImporter();
	mesh = new MeshImporter();
	texture = new TextureImporter();

	scene->Init();
	mesh->Init();
	texture->Init();

	return true;
}

bool ModuleImporter::ImportFile(const char* path)
{
	BROFILER_CATEGORY("ImportFBX_ModuleImporter", Profiler::Color::Yellow);
	bool ret = true;

	std::string normalized_path = path;
	App->fs->NormalizePath(normalized_path);

	std::string extension, file;
	App->fs->SplitFilePath(normalized_path.c_str(), nullptr, &file, &extension);

	if (extension == "fbx" || extension == "FBX")
		scene->Import(path);

	if (extension == "neko")
		mesh->Load(path);

	return ret;
}

bool ModuleImporter::CleanUp()
{
	return true;
}

void ModuleImporter::CreateShape(shape_type type, uint sl, uint st)
{
	BROFILER_CATEGORY("CreateShapes_ModuleImporter", Profiler::Color::Orange);

	GameObject* obj;
	std::string name;
	par_shapes_mesh_s* shape = nullptr;

	//Create shape
	switch (type)
	{
	case SHAPE_CUBE:
		shape = par_shapes_create_cube();
		name = "Cube ";
		break;
	case SHAPE_SPHERE:
		shape = par_shapes_create_parametric_sphere(sl, st);
		name = "Sphere ";
		break;
	case SHAPE_CYLINDER:
		shape = par_shapes_create_cylinder(sl, st);
		name = "Cylinder ";
		break;
	case SHAPE_CONE:
		shape = par_shapes_create_cone(sl, st);
		name = "Cone ";
		break;
	case SHAPE_PLANE:
		shape = par_shapes_create_plane(sl, st);
		name = "Plane ";
		break;
	case SHAPE_TORUS:
		shape = par_shapes_create_torus(sl, st, 0.5f);
		name = "Torus ";
		break;
	case SHAPE_OCTAHEDRON:
		shape = par_shapes_create_octahedron();
		name = "Octahedron ";
		break;
	default:
		LOG("Shape type incorrect or inexistent!");
		break;
	}

	obj = App->viewport->CreateGameObject(name.c_str());
	obj->SetType(OBJECT_PARSHAPE);

	if (!shape || !obj)
		return;

	//Create Component Transform
	ComponentTransform* trans = (ComponentTransform*)obj->CreateComponent(COMPONENT_TRANSFORM);
	trans->local_position.x = 0;
	trans->local_position.y = 0;
	trans->local_position.z = 0;

	par_shapes_scale(shape, 1.0f, 1.0f, 1.0f);
	trans->local_scale.x = 1;
	trans->local_scale.y = 1;
	trans->local_scale.z = 1;

	trans->local_rotation = Quat::identity;
	trans->local_rotation_euler = trans->local_rotation.ToEulerXYZ() * RADTODEG;

	//Create Component Mesh
	ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(COMPONENT_MESH);
	for (uint i = 0; i < shape->npoints * 3;)
	{
		mesh->vertices.push_back(float3(shape->points[i], shape->points[i + 1], shape->points[i + 2]));
		if (shape->normals)
			mesh->normals_face.push_back(float3(shape->normals[i], shape->normals[i + 1], shape->normals[i + 2]));
		i += 3;
	}

	mesh->index.insert(mesh->index.end(), &shape->triangles[0], &shape->triangles[shape->ntriangles * 3]);
	
	mesh->par_shape = true;
	mesh->UV_coord = shape->tcoords;
	mesh->UV_num = 2;
	mesh->transform = trans;

	mesh->CreateLocalAABB();
	mesh->GetGlobalAABB();
	mesh->GenerateBuffers();

	par_shapes_free_mesh(shape);
}