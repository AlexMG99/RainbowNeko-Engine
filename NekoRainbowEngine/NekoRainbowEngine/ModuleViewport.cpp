#include "Globals.h"
#include "GL/include/glew.h"
#include "par/par_shapes.h"
#include "Parson/parson.h"

#include "Application.h"
#include "ModuleViewport.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"

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
	//camera_test = CreateGameObject("Camera", root_object);
	//camera_test->CreateComponent(COMPONENT_CAMERA);
	scene = new Scene();
	App->importer->ImportFile("./Assets/BakerHouse.fbx");
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
		App->camera->FocusObject(selected_object);

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		App->camera->ChangeCamera(camera_test->GetComponentCamera());

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		App->camera->SetSceneCamera();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		SaveScene();

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		LoadScene(scene);


	return UPDATE_CONTINUE;
}

update_status ModuleViewport::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Update_ModuleViewport", Profiler::Color::DeepSkyBlue);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearStencil(0);

	if(draw_grid)
		DrawGrid(2,100);

	root_object->Update();

	return UPDATE_CONTINUE;
}

bool ModuleViewport::CleanUp()
{

	return true;
}

void ModuleViewport::DrawGrid(uint separation, uint lines)
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

bool ModuleViewport::LoadScene(Scene* scn)
{
	Scene go_scene = scn->GetArray("GameObjects");
	Scene go;
	int i = 0;

	while (i !=-1)
	{ 
		go = go_scene.GetSectionArray(i);
		LoadGameObject(go);
		go = go_scene.GetSectionArray(i+1);
		LoadGameObject(go);
		i = -1;
	}
	
	return true;
}

bool ModuleViewport::LoadGameObject(Scene scn)
{
	GameObject* new_obj = CreateGameObject(scn.GetString("Name"));
	new_obj->SetId(scn.GetDouble("ID"));
	new_obj->parent_id = scn.GetDouble("ParentID");
	new_obj->LoadComponents(scn);
	return true;
}


bool ModuleViewport::SaveScene()
{
	bool ret = true;

	Scene go_scene = scene->AddArray("GameObjects");
	
	int num = 0;

	for (auto it_child = root_object->children.begin(); it_child != root_object->children.end(); it_child++)
	{
		ret = SaveGameObject(go_scene, *it_child, &num);
	}

	num = 0;

	ret = scene->Save("scene_test.json");

	return ret;
}

bool ModuleViewport::SaveGameObject(Scene scn, GameObject* obj, int* num)
{
	bool ret = true;
	Scene s_obj = scn.AddSectionArray(*num);

	ret = s_obj.AddString("Name", obj->GetName());
	ret = s_obj.AddDouble("ID", obj->GetId());
	ret = s_obj.AddDouble("ParentID", obj->GetParent()->GetId());

	Scene s_comp = s_obj.AddArray("Components");
	ret = obj->SaveComponents(s_comp);
	
	//Iterate Childrens
	for (auto it_child = obj->children.begin(); it_child != obj->children.end(); it_child++)
	{
		(*num)++;
		ret = SaveGameObject(scn, *it_child, num);
	}

	return ret;
}

GameObject* ModuleViewport::CreateGameObject(std::string name, GameObject* parent, float3 position, float3 scale, Quat rotation)
{
	GameObject* object = new GameObject();
	ComponentTransform* trans = (ComponentTransform*)object->CreateComponent(COMPONENT_TRANSFORM);
	object->SetName(name.c_str());
	trans->local_position = position;
	trans->local_rotation = rotation;
	trans->local_scale = object->CorrectScale(scale);
	trans->local_rotation_euler = rotation.ToEulerXYZ() * RADTODEG;
	object->SetParent(parent);
	object->SetId();
	return object;
}

void ModuleViewport::DeleteGameObject()
{
	//Check ID

	GameObject* parent = selected_object->GetParent();

	for (auto it_obj = parent->children.begin(); it_obj < parent->children.end();) {
		if (selected_object == (*it_obj)) {
			App->viewport->selected_object->SetSelected(false);
			selected_object = nullptr;
			RELEASE(*it_obj);
			it_obj = parent->children.erase(it_obj);
		}
		else
			it_obj++;
	}
}
