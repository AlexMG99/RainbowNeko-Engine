#include "Globals.h"
#include "Application.h"
#include "Brofiler/Brofiler.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "ComponentCamera.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	BROFILER_CATEGORY("Start_ModuleCamera3D", Profiler::Color::LimeGreen);

	LOG("Setting up the camera");
	bool ret = true;

	actual_camera = new ComponentCamera(COMPONENT_CAMERA, true, nullptr);

	return ret;
}

update_status ModuleCamera3D::Load()
{
	actual_camera->Load();

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

	vec3 newPos(0, 0, 0);

	static float speed = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)

		speed = 2 * actual_camera->base_speed * dt;
	else
		speed = actual_camera->base_speed * dt;

	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos -= actual_camera->Z * speed;
	if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos += actual_camera->Z * speed;

	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos -= actual_camera->X * speed;
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)) newPos += actual_camera->X * speed;

	actual_camera->Position += newPos;
	actual_camera->Reference += newPos;

	// Mouse motion ----------------

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		FocusObject(App->viewport->selected_object);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity_mouse = 10.0f * dt;

		actual_camera->Position -= actual_camera->Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * sensitivity_mouse;

			actual_camera->X = rotate(actual_camera->X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			actual_camera->Y = rotate(actual_camera->Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			actual_camera->Z = rotate(actual_camera->Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * sensitivity_mouse;

			actual_camera->Y = rotate(actual_camera->Y, DeltaY, actual_camera->X);
			actual_camera->Z = rotate(actual_camera->Z, DeltaY, actual_camera->X);

			if (actual_camera->Y.y < 0.0f)
			{
				actual_camera->Z = vec3(0.0f, actual_camera->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				actual_camera->Y = cross(actual_camera->Z, actual_camera->X);
			}
		}
		ImGui::SetMouseCursor(ImGuiMouseCursor_Eye);
		actual_camera->Position = actual_camera->Reference + actual_camera->Z * length(actual_camera->Position);
	}

	//Movement Middle button ----------
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {

		float sensitivity_zoom = 100.0f;

		newPos.y -= App->input->GetMouseYMotion() * sensitivity_zoom * dt;
		newPos += App->input->GetMouseXMotion() * sensitivity_zoom * actual_camera->X * dt;

		actual_camera->Position += newPos * dt;
		actual_camera->Reference += newPos * dt;

		ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
	}

	// Wheel Movement ---------------

	vec3 newPos_mouse(0, 0, 0);
	float speed_mouse = 1.0f;

	newPos_mouse -= App->input->GetMouseZ() * speed_mouse * actual_camera->Z;

	actual_camera->Position += newPos_mouse;
	actual_camera->Reference += newPos_mouse;

	// Recalculate matrix -------------
	actual_camera->CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	actual_camera->Position = Position;
	actual_camera->Reference = Reference;

	actual_camera->Z = normalize(Position - Reference);
	actual_camera->X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), actual_camera->Z));
	actual_camera->Y = cross(actual_camera->Z, actual_camera->X);

	if (!RotateAroundReference)
	{
		actual_camera->Reference = actual_camera->Position;
		actual_camera->Position += actual_camera->Z * 0.05f;
	}

	actual_camera->CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	actual_camera->Reference = Spot;

	actual_camera->Z = normalize(actual_camera->Position - actual_camera->Reference);
	actual_camera->X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), actual_camera->Z));
	actual_camera->Y = cross(actual_camera->Z, actual_camera->X);

	actual_camera->CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	actual_camera->Position += Movement;
	actual_camera->Reference += Movement;

	actual_camera->CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::MoveTo(const vec3 &Pos)
{
	actual_camera->Position = Pos;
	actual_camera->Reference = Pos;

	actual_camera->CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::SetCameraToCenter()
{
	App->camera->MoveTo(vec3(7.0f, 7.0f, 7.0f));
	App->camera->LookAt(vec3(0, 0, 0));
}

void ModuleCamera3D::FocusObject(GameObject* obj)
{
	if (obj)
	{
		ComponentTransform* trans = obj->GetComponentTransform();
		if ((actual_camera->Reference.x != trans->local_position.x) && (actual_camera->Reference.y != trans->local_position.y) && (actual_camera->Reference.z != trans->local_position.z)) {
			actual_camera->Reference = vec3(trans->local_position[0], trans->local_position[1], trans->local_position[2]);
			LookAt(actual_camera->Reference);
		}
	}
}