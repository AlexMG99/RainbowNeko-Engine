#include "Globals.h"
#include "Application.h"
#include "Brofiler/Brofiler.h"
#include "ModuleCameraEditor.h"
#include "GameObject.h"
#include "ComponentCamera.h"


ModuleEditorCamera::ModuleEditorCamera(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditorCamera::~ModuleEditorCamera()
{}

// -----------------------------------------------------------------
bool ModuleEditorCamera::Start()
{
	BROFILER_CATEGORY("Start_ModuleCamera3D", Profiler::Color::LimeGreen);

	LOG("Setting up the camera");
	bool ret = true;

	camera = new ComponentCamera(COMPONENT_CAMERA, true, nullptr);

	return ret;
}

update_status ModuleEditorCamera::Load()
{
	camera->Load();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
bool ModuleEditorCamera::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

ComponentCamera * ModuleEditorCamera::GetCamera() const
{
	return camera;
}

// -----------------------------------------------------------------
update_status ModuleEditorCamera::Update(float dt)
{
	BROFILER_CATEGORY("Update_ModuleCamera3D", Profiler::Color::DeepSkyBlue);

	//Keyboard Movement
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == SDL_KEYDOWN)
		Move(dt);

	// Mouse
	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		FocusObject(App->viewport->selected_object);

		int motion_x = App->input->GetMouseXMotion();
		int motion_y = App->input->GetMouseYMotion();

		if (motion_x != 0 || motion_y != 0)
		{
			float dx = (float)-motion_x * rot_speed * dt;
			float dy = (float)-motion_y * rot_speed * dt;

			Orbit(dx, dy);
		}
	}

	//Movement Middle button
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {
		float2 mouse_motion = { App->input->GetMouseXMotion() * move_speed * dt,App->input->GetMouseYMotion() * move_speed * dt };
		Move(mouse_motion.x, mouse_motion.y);
	}

	// Wheel Movement
	int wheel = App->input->GetMouseZ();
	if(wheel != 0)
		Zoom(wheel * zoom_speed * dt);

	return UPDATE_CONTINUE;
}

void ModuleEditorCamera::LookAt(float dx, float dy)
{
	//// x motion make the camera rotate in Y absolute axis (0,1,0) (not local)
	//if (dx != 0.f)
	//{
	//	Quat q = Quat::RotateY(dx);
	//	dummy->frustum.front = q.Mul(dummy->frustum.front).Normalized();
	//	// would not need this is we were rotating in the local Y, but that is too disorienting
	//	dummy->frustum.up = q.Mul(dummy->frustum.up).Normalized();
	//}

	//// y motion makes the camera rotate in X local axis, with tops
	//if (dy != 0.f)
	//{
	//	Quat q = Quat::RotateAxisAngle(dummy->frustum.WorldRight(), dy);

	//	float3 new_up = q.Mul(dummy->frustum.up).Normalized();

	//	if (new_up.y > 0.0f)
	//	{
	//		dummy->frustum.up = new_up;
	//		dummy->frustum.front = q.Mul(dummy->frustum.front).Normalized();
	//	}
	//}
}

// -----------------------------------------------------------------
void ModuleEditorCamera::LookAt(const vec3 &Spot)
{
	vec3 Z = normalize(vec3(camera->frustum.pos.x, camera->frustum.pos.y, camera->frustum.pos.z) - Spot);
	vec3 X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	vec3 Y = cross(Z, X);

	camera->frustum.front = { Z.x,Z.y,Z.z };
	camera->frustum.up = { Y.x,Y.y,Y.z };
}

void ModuleEditorCamera::Zoom(float zoom)
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		zoom *= 2.0f;

	float3 zoom_movement = camera->frustum.front;
	camera->frustum.pos -= zoom_movement * zoom;
}


// -----------------------------------------------------------------
void ModuleEditorCamera::Move(float dt)
{
	Frustum* frustrum = &camera->frustum;

	float final_move_speed = move_speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) final_move_speed *= 2.0f;

	float3 movement = float3::zero;
	float3 front = frustrum->front;
	float3 right = frustrum->WorldRight();

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += front;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= front;

	if (!movement.Equals(float3::zero))
		frustrum->Translate(movement * final_move_speed * dt);
}

void ModuleEditorCamera::Move(float motion_x, float motion_y)
{
	Frustum* frustrum = &camera->frustum;
	frustrum->pos += float3(motion_x, motion_y, 0) * frustrum->WorldRight();

	ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
}

// -----------------------------------------------------------------
void ModuleEditorCamera::MoveTo(const vec3 &Pos)
{
	camera->frustum.pos = float3(Pos.x, Pos.y, Pos.z);
}

void ModuleEditorCamera::Orbit(float motion_x, float motion_y)
{

}


// -----------------------------------------------------------------
void ModuleEditorCamera::SetCameraToCenter()
{
	App->camera->MoveTo(vec3(7.0f, 7.0f, 7.0f));
	App->camera->LookAt(vec3(0, 0, 0));
}

void ModuleEditorCamera::FocusObject(GameObject* obj)
{
	if (obj)
	{
		ComponentTransform* trans = obj->GetComponentTransform();
		vec3 obj_pos = vec3(trans->local_position[0], trans->local_position[1], trans->local_position[2]);
		LookAt(obj_pos);
	}
}