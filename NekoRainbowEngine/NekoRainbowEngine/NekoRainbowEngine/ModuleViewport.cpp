#include "Globals.h"
#include "GL/include/glew.h"

#include "Application.h"
#include "ModuleViewport.h"
#include "ModuleImporter.h"
#include "par/par_shapes.h"

#include "Brofiler/Brofiler.h"

ModuleViewport::ModuleViewport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root_object = new GameObject();
	root_object->SetName("");
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
	ret = App->importer->ImportFBX("../Game/Assets/BakerHouse.fbx", "../Game/Assets/Baker_house.dds");
	CreateGameObjectShape(OBJECT_PARSHAPE, SHAPE_SPHERE);
	return ret;
}

update_status ModuleViewport::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_ModuleViewport", Profiler::Color::SkyBlue);


	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		App->camera->SetCameraToCenter();

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
			DeleteGameObject();

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		App->camera->FocusObject();


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

void ModuleViewport::AddGameObject(GameObject * obj, object_type type, bool active, GameObject * parent)
{
	if (parent == nullptr)
	{
		root_object->AddChildren(obj);
		obj->AddParent(root_object);
	}
	else
	{
		parent->AddChildren(obj);
		obj->AddParent(parent);
	}
	
	obj->SetActive(active);
}

void ModuleViewport::CreateGameObjectShape(object_type type, shape_type s_type, uint slice, uint stack, bool active, GameObject * parent)
{
	App->importer->CreateShape(s_type, slice, stack);
}

void ModuleViewport::DeleteGameObject()
{
	//Iterate Childrens
	for (auto it_obj = root_object->children.begin(); it_obj != root_object->children.end(); ++it_obj) {
		if ((*it_obj)->selected)
		{
			//Check ID
			if ((*it_obj)->GetType() == OBJECT_PARSHAPE && shape_num > 0)
				shape_num--;

			RELEASE(*it_obj);
			it_obj = root_object->children.erase(it_obj);
			return;
		}
	}
}

void ModuleViewport::CheckObjectSelected(GameObject * obj)
{
	for (auto it_obj = root_object->children.begin(); it_obj != root_object->children.end(); ++it_obj) {
		if ((*it_obj) != obj)
			(*it_obj)->selected = false;
	}
}
