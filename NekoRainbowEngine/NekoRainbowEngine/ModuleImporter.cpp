#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "PanelConsole.h"
#include "MathGeoLib/include/Math/float2.h"
#include "glmath.h"
#include <list>
#include <string>

#include"Brofiler/Brofiler.h"

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

	mesh_imp = new MeshImporter();
	texture_imp = new TextureImporter();

	mesh_imp->Init();
	texture_imp->Init();

	return true;
}

bool ModuleImporter::ImportFile(const char* path)
{
	BROFILER_CATEGORY("ImportFBX_ModuleImporter", Profiler::Color::Yellow);
	bool ret = true;

	std::string extension;
	App->fs->SplitFilePath(path, nullptr, nullptr, &extension);

	std::string output_file;

	if (extension == "fbx" || extension == "FBX")
		App->resources->ImportFile(path, resource_type::RESOURCE_MODEL);
	else if (extension == "neko")
	{
		ResourceMesh* mesh = (ResourceMesh*)App->resources->Get(App->resources->ImportFile(path, resource_type::RESOURCE_MESH).GetNumber());
		GameObject* obj = App->viewport->CreateGameObject(mesh->name);
		ComponentMesh* comp_mesh = (ComponentMesh*)obj->CreateComponent(COMPONENT_MESH);
		comp_mesh->transform = obj->GetComponentTransform();
		comp_mesh->AddMesh(mesh);
	}
	else if (extension == "MODEL" || extension == "model")
	{
		ResourceModel* model = (ResourceModel*)App->resources->Get(App->resources->ImportFile(path, resource_type::RESOURCE_MODEL).GetNumber());

		model->Load();
	}
	else if (extension == "png" || extension == "dds" || extension == "jpg" || extension == "PNG" || extension == "DDS" || extension == "JPG" || extension == "TGA" || extension == "tga")
	{
		if (App->viewport->selected_object)
		{
			ComponentTexture* comp_texture = App->viewport->selected_object->GetComponentTexture();
			if (comp_texture)
			{
				ResourceTexture* texture = (ResourceTexture*)App->resources->Get(App->resources->ImportFile(path, resource_type::RESOURCE_TEXTURE).GetNumber());
				if (texture)
				{
					App->resources->ImportAssets(path);
					comp_texture->AddTexture(texture);
					App->viewport->selected_object->GetComponentMesh()->image_id = comp_texture->texture->image_id;
					LOG("Load Texture succesfully with name: %s", texture->file.c_str());
				}
				else
					LOG("Resource not loaded");
			}
			else
				LOG("Object don't found. There's not selected object!");

		}

	}
	else
		LOG("Format not allowed");
		

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
	ComponentMesh* comp_mesh = (ComponentMesh*)obj->CreateComponent(COMPONENT_MESH);
	ResourceMesh* mesh = new ResourceMesh();

	//Load Normals
	if (shape->normals)
	{
		mesh->normals_face = new float3[shape->npoints];
		memcpy(mesh->normals_face, shape->normals, sizeof(float3) * shape->npoints);
	}

	//Load UVs
	if (shape->tcoords)
	{
		mesh->UV_size = shape->npoints;
		mesh->UV_coord = new float2[mesh->UV_size];
		memcpy(mesh->UV_coord, shape->tcoords, sizeof(float2) * mesh->UV_size);
	}
	mesh->GenerateBuffers();

	comp_mesh->transform = trans;
	comp_mesh->AddMesh(mesh);
	obj->GetGlobalAABB();

	par_shapes_free_mesh(shape);
}