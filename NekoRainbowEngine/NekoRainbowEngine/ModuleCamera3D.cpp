#include "Globals.h"
#include "Application.h"
#include "Brofiler/Brofiler.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	BROFILER_CATEGORY("Start_ModuleCamera3D", Profiler::Color::LimeGreen);

	LOG("Setting up the camera");
	bool ret = true;
	
	return ret;
}

update_status ModuleCamera3D::Load()
{
	BROFILER_CATEGORY("Load_ModuleCamera3D", Profiler::Color::LightGray);

	JSON_Object* obj = json_object(App->settings_doc);
	JSON_Object* cam_obj = json_object_get_object(json_object_get_object(obj, "Application"), "Camera");

	base_speed = json_object_get_number(cam_obj, "Speed");
	Position = { (float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "X"),
		(float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "Y"),
		(float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "Z") };

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	BROFILER_CATEGORY("Update_ModuleCamera3D", Profiler::Color::DeepSkyBlue);
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);

	static float speed = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 2 * base_speed;
	else
		speed = base_speed;

	if((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos -= Z * speed * dt;
	if((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos += Z * speed * dt;

	if((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos -= X * speed * dt;
	if((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos += X * speed * dt;

	Position += newPos * dt;
	Reference += newPos * dt;

	// Mouse motion ----------------

	if((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		FocusObject(App->viewport->selected_object);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity_mouse = 10.0f * dt;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * sensitivity_mouse;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * sensitivity_mouse;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}
		ImGui::SetMouseCursor(ImGuiMouseCursor_Eye);
		Position = Reference + Z * length(Position);
	}

	//Movement Middle button ----------
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {

		float sensitivity_zoom = 100.0f;

		newPos.y -= App->input->GetMouseYMotion() * sensitivity_zoom * dt;
		newPos += App->input->GetMouseXMotion() * sensitivity_zoom * X * dt;

		Position += newPos * dt;
		Reference += newPos * dt;

		ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
	}

	// Wheel Movement ---------------

	vec3 newPos_mouse(0, 0, 0);
	float speed_mouse = 100.0f;

	newPos_mouse -= App->input->GetMouseZ() * speed_mouse * Z;

	Position += newPos_mouse * dt;
	Reference += newPos_mouse * dt;


	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
	
	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::MoveTo(const vec3 &Pos)
{
	Position = Pos;
	Reference = Pos;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::SetCameraToCenter()
{
	App->camera->MoveTo(vec3(7.0f, 7.0f, 7.0f));
	App->camera->LookAt(vec3(0, 0, 0));
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::FocusObject(GameObject* obj)
{
	if (obj)
	{
		ComponentTransform* trans = obj->GetComponentTransform();
		if ((Reference.x != trans->local_position.x) && (Reference.y != trans->local_position.y) && (Reference.z != trans->local_position.z)) {
			Reference = vec3(trans->local_position[0], trans->local_position[1], trans->local_position[2]);
			LookAt(Reference);
		}
	}
}

void ModuleCamera3D::FocusObjectImport(GameObject* obj)
{
	if (obj)
	{
		ComponentMesh* mesh = obj->GetComponentMesh();
		if (mesh)
		{
			float3 center = ((mesh->local_AABB.minPoint + mesh->local_AABB.maxPoint) / 2);
			Reference = vec3(center[0], center[1], center[2]);
			LookAt(Reference);
		}
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}