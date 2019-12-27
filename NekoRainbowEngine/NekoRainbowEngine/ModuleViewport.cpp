#include "Globals.h"
#include "GL/include/glew.h"
#include "par/par_shapes.h"
#include "Parson/parson.h"

#include "Application.h"
#include "ModuleViewport.h"
#include "ModuleImporter.h"
#include "ModuleCameraEditor.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentUI.h"
#include "ComponentCheckbox.h"
#include "Scene.h"
#include "PanelScene.h"
#include "PanelPlay.h"
#include "RayCast.h"
#include "ResourceMesh.h"
#include "Time.h"

#include "Assimp/include/anim.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "FBO.h"

#include "Brofiler/Brofiler.h"

ModuleViewport::ModuleViewport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleViewport::~ModuleViewport()
{
	RELEASE(root_object);
	RELEASE(scene);
}

bool ModuleViewport::Start()
{
	BROFILER_CATEGORY("Start_ModuleViewport", Profiler::Color::LimeGreen);

	bool ret = true;
	root_object = CreateGameObject("Root Object");
	camera_game = CreateGameObject("Camera", root_object);
	ComponentCamera* comp_camera = camera_game->CreateComponentCamera(1.0f, 100.0f, 90);

	scene = new Scene(std::string(point + LIBRARY_SCENE_FOLDER + scene_name).c_str());
	if (!scene->GetVRoot())
		scene = new Scene();

	//Init Scene FBO
	scene_fbo = new FBO();
	scene_fbo->Create((uint)App->window->GetWinSize().x, App->window->GetWinSize().y);
	scene_fbo->SetComponentCamera(App->camera->GetSceneCamera());

	//Init Game FBO
	game_fbo = new FBO();
	game_fbo->Create((uint)App->window->GetWinSize().x, App->window->GetWinSize().y);
	game_fbo->SetComponentCamera(camera_game->GetComponentCamera());

	canvas = CreateGameObject("Canvas", camera_game);
	canvas->CreateComponent(COMPONENT_CANVAS, true, 937,760);

	App->fonts->default_font = App->fonts->LoadFont("./Fonts/Roboto.ttf", 120);

	//App->importer->ImportFile("./Assets/Street environment_V01.FBX");
	uint width = comp_camera->frustum.CornerPoint(3).x - comp_camera->frustum.CornerPoint(7).x;
	uint height = comp_camera->frustum.CornerPoint(7).y - comp_camera->frustum.CornerPoint(5).y;
	CreateUIElement("Title", UI_Label, 100, 30, canvas->GetComponentCanvas(), "Menu", canvas, { 80,150,-1 });
	CreateUIElement("Background", UI_Image, width, height, canvas->GetComponentCanvas(), "./Assets/background.jpg", canvas);
	CreateUIElement("PlayButton", UI_Button, 100, 30, canvas->GetComponentCanvas(), "./Assets/button.png", canvas, { 80,120,-1 });
	checkbox = CreateUIElement("VSyncCheckbox", UI_Checkbox, 20, 20, canvas->GetComponentCanvas(), "Puta", canvas, { 20,20,-1 });
	ComponentCheckbox* comp_checkbox = (ComponentCheckbox*)checkbox->GetComponentUI();
	comp_checkbox->SetState(vsync);

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

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		SaveScene();

	if ((App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN))
		LoadScene(scene_name.c_str());

	return UPDATE_CONTINUE;
}

update_status ModuleViewport::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Update_ModuleViewport", Profiler::Color::DeepSkyBlue);
	
	scene_fbo->Bind(App->editor->panel_scene->window_size);
	if(draw_grid)
		DrawGrid(2,100);
	root_object->Update();
	quad_tree.Draw();
	scene_fbo->Unbind();

	game_fbo->Bind(App->editor->panel_play->window_size);
	root_object->Update();
	game_fbo->Unbind();

	return UPDATE_CONTINUE;
}

bool ModuleViewport::CleanUp()
{
	scene_fbo->Unbind();

	RELEASE(scene_fbo);
	RELEASE(game_fbo);

	return true;
}

bool CompareRayCast(RayCast & a, RayCast & b)
{
	return a.distance < b.distance;
}

GameObject* ModuleViewport::MyRayCastIntersection(LineSegment * my_ray, RayCast & hit)
{
	std::vector<RayCast> scn_obj;
	BoxIntersection(root_object, my_ray, scn_obj);

	std::sort(scn_obj.begin(), scn_obj.end(), CompareRayCast);

	GameObject* var = nullptr;
	GameObject* selec = nullptr;

	for (std::vector<RayCast>::iterator iter = scn_obj.begin(); iter != scn_obj.end(); ++iter)
	{
		var = (*iter).go;
		selec = TriangleTest(*my_ray, var);
		if (selec != nullptr)
			break;
	}

	return selec;
}

void ModuleViewport::BoxIntersection(GameObject * obj, LineSegment * ray, std::vector<RayCast>& scene_obj)
{
	if (obj->GetComponentMesh() != nullptr)
	{
		if (obj->GetComponentTransform()->ItIntersect(*ray))
		{
			ComponentMesh* comp_mesh = obj->GetComponentMesh();
			RayCast hit(obj);
			float near_hit, far_hit;
			if (ray->Intersects(comp_mesh->BB_mesh.global_OBB, near_hit, far_hit))
			{
				hit.distance = near_hit;
				scene_obj.push_back(hit);
			}
		}
	}
	for (auto iter = obj->children.begin(); iter != obj->children.end(); ++iter)
	{
		BoxIntersection((*iter), ray, scene_obj);
	}
}

GameObject* ModuleViewport::TriangleTest(LineSegment& ray, GameObject* obj)
{
	bool intersected = false;
	ComponentMesh* mesh = obj->GetComponentMesh();

	if (mesh != nullptr)
	{
		ComponentTransform* trans = obj->GetComponentTransform();

		for (uint i = 0; i < mesh->mesh->index_size; i += 3)
		{
			uint index_a, index_b, index_c;

			index_a = mesh->mesh->index[i] * 3;
			float3 point_a(mesh->mesh->vertices[index_a]);

			index_b = mesh->mesh->index[i + 1] * 3;
			float3 point_b(mesh->mesh->vertices[index_b]);

			index_c = mesh->mesh->index[i + 2] * 3;
			float3 point_c(mesh->mesh->vertices[index_c]);

			Triangle triangle_to_check(point_a, point_b, point_c);
			triangle_to_check.Transform(trans->GetGlobalTransformMatrix());

			if (ray.Intersects(triangle_to_check, nullptr, nullptr))
			{
				intersected = true;
				return obj;
				break;
			}
		}

		if (!intersected)
			return nullptr;
	}

	else if (obj->children.empty())
		return obj;
	else
		return nullptr;
}

void ModuleViewport::DrawGrid(uint separation, uint lines)
{
	glColor3f(255, 255, 255);

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

void ModuleViewport::GuizControls()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) 
		guizmo_op = ImGuizmo::OPERATION::TRANSLATE;
	
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		guizmo_op = ImGuizmo::OPERATION::ROTATE;

	if(App->input->GetKey(SDL_SCANCODE_R)==KEY_DOWN)
		guizmo_op = ImGuizmo::OPERATION::SCALE;
	
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) 
		guizmo_mode = ImGuizmo::MODE::WORLD;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) 
	    guizmo_mode = ImGuizmo::MODE::LOCAL;
}

void ModuleViewport::GuizLogic()
{
	if (App->viewport->selected_object != nullptr && !App->viewport->selected_object->is_static)
	{
		ComponentTransform* transform = (ComponentTransform*)App->viewport->selected_object->GetComponentTransform();

		float4x4 model = transform->global_matrix.Transposed();

		float4x4 view_transposed = App->camera->camera->frustum.ViewMatrix();
		view_transposed = view_transposed.Transposed();

		float4x4 projection_transposed = App->camera->camera->frustum.ProjectionMatrix().Transposed();
		float4x4 object_transform_matrix = transform->global_matrix.Transposed();

		ImGuizmo::SetRect(App->editor->panel_scene->pos_x, App->editor->panel_scene->pos_y, App->editor->panel_scene->width, App->editor->panel_scene->height);
		ImGuizmo::SetDrawlist();
		ImGuizmo::Manipulate(view_transposed.ptr(), projection_transposed.ptr(), guizmo_op, guizmo_mode, object_transform_matrix.ptr());
	
		if (ImGuizmo::IsUsing())
		{
			ComponentTransform* trans_parent = App->viewport->selected_object->GetParent()->GetComponentTransform();
			if (App->viewport->selected_object->GetParent() != App->viewport->root_object)
			{
				transform->SetGlobalTransform(trans_parent->global_transformation.Inverted()*object_transform_matrix.Transposed());
				transform->UpdateComponents();
			}
			else
			{
				transform->SetGlobalTransform(object_transform_matrix.Transposed());
				transform->UpdateComponents();
			}
		}

		if (ImGuizmo::IsOver())
			is_over_guizmo = true;
		else
			is_over_guizmo = false;
	}
}

void ModuleViewport::Play_Time()
{
	Time::Start();
	SaveScene();
	App->editor->ChangeActualWindow(true);
	camera_culling = true;
}

void ModuleViewport::Stop_Time()
{
	Time::Stop();
	LoadScene(scene_name.c_str());

	App->editor->ChangeActualWindow(false);
	camera_culling = false;
}

bool ModuleViewport::LoadScene(const char* name)
{
	ResetScene();

	scene = new Scene(std::string(point + LIBRARY_SCENE_FOLDER + name).c_str());

	Scene go_scene = scene->GetArray("GameObjects");
	int i = 0;

	while (i !=-1)
	{ 
		if (go_scene.IsArraySection(i))
		{
			LoadGameObject(go_scene.GetSectionArray(i));
			i++;
		}
		else
			i = -1;
		
	}

	if(!root_object->children.empty())
		ReorganizeHierarchy();

	game_fbo->SetComponentCamera(camera_game->GetComponentCamera());

	return true;
}

bool ModuleViewport::LoadGameObject(Scene scn)
{
	if (!scn.IsString("Name"))
	{
		LOG("Scene don't load");
		return false;
	}

	GameObject* new_obj = CreateGameObject(scn.GetString("Name"));

	new_obj->is_static = scn.GetBool("Static");
	new_obj->SetType((object_type)scn.GetInt("Type"));
	new_obj->SetId(scn.GetDouble("ID"));
	new_obj->parent_id = scn.GetDouble("ParentID");
	new_obj->LoadComponents(scn);

	if (new_obj->GetName() == "Camera")
	{
		camera_game = new_obj;
	}

	return true;
}


bool ModuleViewport::SaveScene()
{
	bool ret = true;
	Scene go_scene = scene->AddArray("GameObjects");
	
	int num = -1;
	for (auto it_child = root_object->children.begin(); it_child != root_object->children.end(); it_child++)
	{
		ret = SaveGameObject(go_scene, *it_child, &num);
	}
	num = -1;

	ret = scene->Save(std::string(point + LIBRARY_SCENE_FOLDER + scene_name).c_str());

	return ret;
}

bool ModuleViewport::SaveGameObject(Scene scn, GameObject* obj, int* num)
{
	bool ret = true;
	(*num)++;

	Scene s_obj = scn.AddSectionArray(*num);

	ret = s_obj.AddBool("Static", obj->is_static);
	ret = s_obj.AddString("Name", obj->GetName());
	ret = s_obj.AddInt("Type", obj->GetType());
	ret = s_obj.AddDouble("ID", obj->GetId());
	ret = s_obj.AddDouble("ParentID", obj->GetParent()->GetId());
	Scene s_comp = s_obj.AddArray("Components");
	ret = obj->SaveComponents(s_comp);
	
	//Iterate Childrens
	for (auto it_child = obj->children.begin(); it_child != obj->children.end(); it_child++)
	{
		ret = SaveGameObject(scn, *it_child, num);
	}

	return ret;
}

void ModuleViewport::ReorganizeHierarchy()
{
	if (root_object->children.empty())
		return;

	for (auto it_child = root_object->children.begin(); it_child != root_object->children.end() - 1;)
	{
		auto it_next = (it_child + 1);
		if ((*it_child)->IsParentID((*it_next)->parent_id))
		{
			(*it_next)->SetParent(*it_child);
			root_object->RemoveChild(*it_next);
		}
		else
		{
			GameObject* it_new = (*it_child)->GetIteratorChild((*it_next)->parent_id);
			if (it_new)
			{
				(*it_next)->SetParent(it_new);
				root_object->RemoveChild(*it_next);
			}
			else
				it_child++;
		}
	}
}

bool ModuleViewport::ResetScene()
{
	selected_object = nullptr;

	RELEASE_ARRAY_LIST(root_object->children);

	RELEASE(scene);

	return true;
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

GameObject * ModuleViewport::CreateUIElement(char*  name, UI_type type, uint width, uint height, ComponentCanvas* canvas, char* str, GameObject* parent, float3 position, float3 scale, Quat rotation)
{
	GameObject* object = new GameObject();
	ComponentTransform* trans = (ComponentTransform*)object->CreateComponent(COMPONENT_TRANSFORM);
	object->SetParent(parent);
	object->SetName(name);
	trans->local_position = position;
	trans->local_rotation = rotation;
	trans->local_scale = object->CorrectScale(scale);
	trans->local_rotation_euler = rotation.ToEulerXYZ() * RADTODEG;
	object->CreateComponentUI(type, width, height, canvas, str);
	object->SetId();
	return object;
}

void ModuleViewport::DeleteGameObject()
{
	//Check ID
	if (selected_object == camera_game)
		return;

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
