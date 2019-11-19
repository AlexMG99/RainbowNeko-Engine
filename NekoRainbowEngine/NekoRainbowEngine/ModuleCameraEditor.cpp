#include "Globals.h"
#include "Application.h"
#include "GL/include/glew.h"
#include "Brofiler/Brofiler.h"
#include "ModuleCameraEditor.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "RayCast.h"
#include "MathGeoLib/include/MathGeoLib.h"


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

	camera = scene_camera = new ComponentCamera(COMPONENT_CAMERA, true, nullptr);

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
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
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

		ImGui::SetMouseCursor(ImGuiMouseCursor_Eye);
	}

	//Movement Middle button
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {
		float2 mouse_motion = { App->input->GetMouseXMotion() * move_mouse_speed * dt,App->input->GetMouseYMotion() * move_mouse_speed * dt };
		Move(mouse_motion.x, mouse_motion.y);
	}

	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
		
		vec2 mouse_pos = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
		LOG("Mouse Pos: %f, %f", mouse_pos.x, mouse_pos.y);
		mouse_pos = normalize(mouse_pos);
		LOG("Normalized Mouse Pos: %f, %f", mouse_pos.x, mouse_pos.y);


		picking = camera->frustum.UnProjectLineSegment(mouse_pos.x, mouse_pos.y);
		
		RayCast ray;

		if (App->viewport->MyRayCastIntersection(&picking, ray))
		{
			picked_obj->SetSelected(ray.trans);
		}
	}

	// Wheel Movement
	int wheel = App->input->GetMouseZ();
	if(wheel != 0)
		Zoom(wheel * zoom_speed * dt);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleEditorCamera::LookAt(const float3 &Spot)
{
	camera->Reference = Spot;
	camera->Look(Spot);
}

void ModuleEditorCamera::ChangeCamera(ComponentCamera* camera)
{
	if(!this->camera->my_go)
		scene_camera = this->camera;

	camera->update_proj = true;
	this->camera = camera;
}

void ModuleEditorCamera::SetSceneCamera()
{
	camera = scene_camera;
	camera->update_proj = true;
}

void ModuleEditorCamera::Zoom(float zoom)
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		zoom *= 2.0f;

	float3 zoom_movement = camera->frustum.front * zoom;
	camera->frustum.pos += zoom_movement;
	camera->Reference += zoom_movement;

	camera->UpdateFrustum(true);

	ImGui::SetMouseCursor(ImGuiMouseCursor_Zoom);
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
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement -= front;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement += front;

	if (!movement.Equals(float3::zero))
	{
		frustrum->Translate(movement * final_move_speed * dt);
		camera->Reference += movement * final_move_speed * dt;
	}

	camera->UpdateFrustum(true);
}

void ModuleEditorCamera::Move(float motion_x, float motion_y)
{
	Frustum* frustrum = &camera->frustum;
	frustrum->pos += -frustrum->up * motion_y + frustrum->WorldRight()*motion_x;
	camera->Reference += -frustrum->up * motion_y + frustrum->WorldRight()*motion_x;

	camera->UpdateFrustum(true);

	ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
}

// -----------------------------------------------------------------
void ModuleEditorCamera::MoveTo(const vec3 &Pos)
{
	camera->frustum.pos = float3(Pos.x, Pos.y, Pos.z);
	camera->Reference = float3(Pos.x, Pos.y, Pos.z);

	camera->UpdateFrustum(true);

}

void ModuleEditorCamera::Orbit(float motion_x, float motion_y)
{
	camera->frustum.pos -= camera->Reference;

	if (motion_x != 0)
	{
		vec3 rot_Y = rotate(vec3(camera->frustum.up.x, camera->frustum.up.y, camera->frustum.up.z), motion_x, vec3(0.0f, 1.0f, 0.0f));
		vec3 rot_Z = rotate(vec3(camera->frustum.front.x, camera->frustum.front.y, camera->frustum.front.z), motion_x, vec3(0.0f, 1.0f, 0.0f));

		camera->frustum.up = float3(rot_Y.x, rot_Y.y, rot_Y.z);
		camera->frustum.front = float3(rot_Z.x, rot_Z.y, rot_Z.z);
	}

	if (motion_y != 0)
	{
		vec3 rot_Y = rotate(vec3(camera->frustum.up.x, camera->frustum.up.y, camera->frustum.up.z), motion_y, vec3(camera->frustum.WorldRight().x, camera->frustum.WorldRight().y, camera->frustum.WorldRight().z));
		vec3 rot_Z = rotate(vec3(camera->frustum.front.x, camera->frustum.front.y, camera->frustum.front.z), motion_y, vec3(camera->frustum.WorldRight().x, camera->frustum.WorldRight().y, camera->frustum.WorldRight().z));

		camera->frustum.up = float3(rot_Y.x, rot_Y.y, rot_Y.z);
		camera->frustum.front = float3(rot_Z.x, rot_Z.y, rot_Z.z);

		if (camera->frustum.up.y < 0.0f)
		{
			camera->frustum.front = float3(0.0f, camera->frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			vec3 cross_vec = cross(vec3(camera->frustum.front.x, camera->frustum.front.y, camera->frustum.front.z), vec3(camera->frustum.WorldRight().x, camera->frustum.WorldRight().y, camera->frustum.WorldRight().z));
			camera->frustum.up = float3(cross_vec.x, cross_vec.y, cross_vec.z);
		}
	}
	camera->frustum.pos = camera->Reference - camera->frustum.front * length(vec3(camera->frustum.pos.x, camera->frustum.pos.y, camera->frustum.pos.z));

	camera->UpdateFrustum(true);
}


// -----------------------------------------------------------------
void ModuleEditorCamera::SetCameraToCenter()
{
	App->camera->MoveTo(vec3(7.0f, 7.0f, 7.0f));
	App->camera->LookAt(float3(0, 0, 0));

	camera->UpdateFrustum(true);
}

void ModuleEditorCamera::FocusObject(GameObject* obj)
{
	if (obj)
	{
		ComponentTransform* trans = obj->GetComponentTransform();
		camera->Reference = float3(trans->local_position[0], trans->local_position[1], trans->local_position[2]);
		LookAt(camera->Reference);
	}

	camera->UpdateFrustum(true);
}