#include "Application.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "GL/include/glew.h"
#include "ComponentUI.h"
#include "PanelPlay.h"

ComponentUI::ComponentUI(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* str) :Component(comp_type, act, obj)
{
	this->canvas = canvas;
	this->ui_type = type;
	height = h;
	width = w;
	dragable = true;

	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	pos_x = comp_trans->local_position.x;
	pos_y = comp_trans->local_position.y;

	if (type != UI_Label)
	{
		ComponentMesh* comp_mesh = (ComponentMesh*)my_go->CreateComponent(COMPONENT_MESH);

		float3* vertex = new float3[4];
		vertex[0] = float3(0, height, 0);
		vertex[1] = float3(width, height, 0);
		vertex[3] = float3(width, 0, 0);
		vertex[2] = float3(0, 0, 0);

		float2* UV_coord = new float2[4];

		if (type != UI_Character)
		{
			UV_coord[1] = float2(0, 1);
			UV_coord[0] = float2(1, 1);
			UV_coord[2] = float2(1, 0);
			UV_coord[3] = float2(0, 0);
		}
		else
		{
			UV_coord[3] = float2(0, 1);
			UV_coord[2] = float2(1, 1);
			UV_coord[0] = float2(1, 0);
			UV_coord[1] = float2(0, 0);
		}

		mesh = App->resources->FindMeshUI(my_go->GetName().c_str());

		if (!mesh)
		{
			mesh = new ResourceMesh();
			comp_mesh->AddMesh(mesh->CreateMesh(vertex, UV_coord, my_go->GetName().c_str()));
		}
		else
			comp_mesh->AddMesh(mesh);
	}
	
};

bool ComponentUI::OnClicked()
{
	if (dragable)
	{
		Move();
	}

	return true;
}

bool ComponentUI::OnRelease()
{
	if(my_go->GetComponentMesh())
		my_go->GetComponentMesh()->ChangeColor(vec4(1, 1, 1, 1));
	return true;
}

void ComponentUI::Move()
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();

	comp_trans->local_position += float3(-App->input->GetMouseXMotion() * App->GetDT() * 10, -App->input->GetMouseYMotion()* App->GetDT() * 10, 0);
	pos_x = comp_trans->local_position.x;
	pos_y = comp_trans->local_position.y;
}

void ComponentUI::DebugDraw()
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();

	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = comp_trans->position;

	float3 v1 = float3(pos.x, pos.y, pos.z);
	float3 v2 = float3(pos.x + width, pos.y, pos.z);
	float3 v3 = float3(pos.x, pos.y + height, pos.z);
	float3 v4 = float3(pos.x + width, pos.y + height, pos.z);

	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v4.x, v4.y, v4.z);
	glVertex3f(v3.x, v3.y, v3.z);

	glEnd();
}

void ComponentUI::UpdateTransform()
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	comp_trans->local_position = { comp_trans->local_position.x, comp_trans->local_position.y, comp_trans->local_position.z };
	comp_trans->GetGlobalTransformMatrix();
}

bool ComponentUI::Update()
{
	if (!App->viewport->is_over_game)
		return false;

	UILogic();

	switch (state)
	{
	case UI_Idle:
		break;
	case UI_Hover:
		OnHover();
		break;
	case UI_Click:
		OnClick();
		break;
	case UI_Clicked:
		OnClicked();
		break;
	case UI_Release:
		OnRelease();
		break;
	default:
		break;
	}

	UpdateUI(App->GetDT());

	return true;
}

void ComponentUI::UILogic()
{
	float2 origin = float2(App->editor->panel_play->cursor_pos.x / App->editor->panel_play->window_size.x, App->editor->panel_play->cursor_pos.y / App->editor->panel_play->window_size.y);
	origin.x = (origin.x - 0.5F) * 2;
	origin.y = -(origin.y - 0.5F) * 2;

	ImVec2 ratio = ImVec2(App->editor->panel_play->window_size.x / canvas->width, App->editor->panel_play->window_size.y / canvas->height);
	float2 mouse_pos = float2(-App->editor->panel_play->cursor_pos.x / ratio.x, -App->editor->panel_play->cursor_pos.y / ratio.y);

	switch (state)
	{
	case UI_Idle:
		if (CheckMouseInside(mouse_pos))
			state = UI_Hover;
		break;
	case UI_Hover:
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			state = UI_Click;
		if (!CheckMouseInside(mouse_pos))
			state = UI_Release;
		break;
	case UI_Click:
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			state = UI_Clicked;
		break;
	case UI_Clicked:
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !CheckMouseInside(mouse_pos))
			state = UI_Release;
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && CheckMouseInside(mouse_pos))
			state = UI_Hover;
		break;
	case UI_Release:
		state = UI_Idle;
		break;

	}
}

bool ComponentUI::Fade()
{
	return my_go->GetComponentMesh()->Fade(App->GetDT());
}

bool ComponentUI::CheckMouseInside(float2 mouse_pos)
{
	return (mouse_pos.x >= pos_x && mouse_pos.x <= pos_x + width && mouse_pos.y >= pos_y && mouse_pos.y <= pos_y + height);
}
