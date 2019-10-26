#include "Globals.h"
#include "GL/include/glew.h"

#include "Application.h"
#include "ModuleViewport.h"
#include "ModuleImporter.h"
#include "par/par_shapes.h"
#include "Component.h"

#include "Brofiler/Brofiler.h"

ModuleViewport::ModuleViewport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleViewport::~ModuleViewport()
{
	RELEASE(root_object);
}

bool ModuleViewport::Start()
{
	BROFILER_CATEGORY("Start_ModuleViewport", Profiler::Color::LimeGreen);

	bool ret = true;
	root_object = CreateGameObject("Root Object");
	ret = App->importer->ImportFBX("../Game/Assets/BakerHouse.fbx", "../Game/Assets/Baker_house.dds");
	//App->importer->CreateShape(SHAPE_SPHERE, 10, 10);
	return ret;
}

update_status ModuleViewport::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_ModuleViewport", Profiler::Color::SkyBlue);


	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		App->camera->SetCameraToCenter();

	if ((App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) && selected_object)
			DeleteGameObject();

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		App->camera->FocusObject(root_object);


	return UPDATE_CONTINUE;
}

update_status ModuleViewport::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Update_ModuleViewport", Profiler::Color::DeepSkyBlue);

	CreateGrid(2,100);

	root_object->Update();
	return UPDATE_CONTINUE;
}

bool ModuleViewport::CleanUp()
{

	return true;
}

void ModuleViewport::CreateGrid(uint separation, uint lines)
{
	//Vertex Point
	glPointSize(10.0f);

	glEnable(GL_POINT_SMOOTH);

	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();

	glDisable(GL_POINT_SMOOTH);

	glPointSize(1.0f);

	//Axis X,Y,Z
	glLineWidth(7.0);

	glBegin(GL_LINES);

	glColor3f(255, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1.5, 0, 0);

	glColor3f(0, 255, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1.5, 0);

	glColor3f(0, 0, 255);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1.5);

	glColor3f(255, 255, 255);
	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);
	for (int i = 0; i <= lines; i++) {
		glVertex3f(-((separation*lines)*0.5) + i*separation, 0, -((separation*lines)*0.5));
		glVertex3f(-((separation*lines)*0.5) +i*separation, 0, (separation*lines)*0.5);
		glVertex3f(-((separation*lines)*0.5), 0, -((separation*lines)*0.5) + i*separation);
		glVertex3f((separation*lines)*0.5, 0, -((separation*lines)*0.5) + i*separation);
	}

	glEnd();

}

GameObject* ModuleViewport::CreateGameObject(std::string name, GameObject* parent, float3 position, float3 scale, Quat rotation)
{
	GameObject* object = new GameObject();
	ComponentTransform* trans = (ComponentTransform*)object->CreateComponent(COMPONENT_TRANSFORM);
	object->SetName(name.c_str());
	trans->local_position = position;
	trans->local_rotation = rotation;
	trans->local_scale = object->GetScale(scale);
	trans->local_rotation_euler = rotation.ToEulerXYZ() * RADTODEG;
	object->SetParent(parent);
	
	return object;
}

void ModuleViewport::DeleteGameObject()
{
	//Check ID
	if (selected_object->GetType() == OBJECT_PARSHAPE && shape_num > 0)
		shape_num--;

	GameObject* parent = selected_object->GetParent();

	for (auto it_obj = parent->children.begin(); it_obj < parent->children.end();) {
		if (selected_object == (*it_obj)) {
			RELEASE(*it_obj);
			it_obj = parent->children.erase(it_obj);
			selected_object = nullptr;
		}
		else
			it_obj++;
	}
}