#include "Application.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "GL/include/glew.h"
#include "ComponentUI.h"
#include "PanelPlay.h"

ComponentUI::ComponentUI(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* str, uint x, uint y) :Component(comp_type, act, obj)
{
	this->canvas = canvas;
	this->ui_type = type;
	height = h;
	width = w;
	fill_color = { 1,1,1,1 };

	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	if (x != 0 || y != 0)
	{
		pos_x = x;
		pos_y = y;
	}
	else
	{
		pos_x = comp_trans->local_position.x;
		pos_y = comp_trans->local_position.y;
	}

	if (type != UI_Label)
	{
		float3* vertex = new float3[4];
		panel.vertex[0] = float3(pos_x, pos_y + height, comp_trans->local_position.z);
		panel.vertex[1] = float3(pos_x + width, pos_y + height, comp_trans->local_position.z);
		panel.vertex[3] = float3(pos_x + width, pos_y, comp_trans->local_position.z);
		panel.vertex[2] = float3(pos_x, pos_y, comp_trans->local_position.z);

		float2* UV_coord = new float2[4];

		if (type == UI_Character)
		{
			panel.uv[0] = float2(0, 1);
			panel.uv[1] = float2(1, 1);
			panel.uv[3] = float2(1, 0);
			panel.uv[2] = float2(0, 0);
		}
		else
		{
			panel.uv[2] = float2(0, 1);
			panel.uv[3] = float2(1, 1);
			panel.uv[1] = float2(1, 0);
			panel.uv[0] = float2(0, 0);
		}

		panel.GenerateBuffers();
	}
	
}
ComponentUI::~ComponentUI()
{
	RELEASE(canvas);
}
;

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
	fill_color = vec4(1, 1, 1, fill_color.w);
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
	glColor4f(fill_color.x, fill_color.y, fill_color.z, fill_color.w);

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

void ComponentUI::Draw()
{
	DebugDraw();

	if (App->viewport->is_game_mode && App->viewport->draw_ui)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, App->editor->panel_play->window_size.x, App->editor->panel_play->window_size.y, 0.0, 1.0, -1.0);

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	if (!App->viewport->is_game_mode)
	{
		glPushMatrix();
		glMultMatrixf((float*)&my_go->GetComponentTransform()->GetGlobalTransformMatrix().Transposed());
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(fill_color.x, fill_color.y, fill_color.z, fill_color.w);

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, panel.textureID);

	glBindBuffer(GL_ARRAY_BUFFER, panel.buffer[0]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, panel.buffer[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, panel.buffer[1]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	if (!App->viewport->is_game_mode)
		glPopMatrix();

}

void ComponentUI::UpdateTransform()
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	comp_trans->local_position = { comp_trans->local_position.x, comp_trans->local_position.y, comp_trans->local_position.z };
	comp_trans->GetGlobalTransformMatrix();
}

bool ComponentUI::Update()
{
	Draw();

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

	float2 mouse_pos = float2(App->editor->panel_play->cursor_pos.x, App->editor->panel_play->cursor_pos.y);

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

void ComponentUI::ChangeColor(vec4 new_color)
{
	for (auto it_go = my_go->children.begin(); it_go != my_go->children.end(); it_go++)
	{
		if((*it_go)->GetComponentUI())
			(*it_go)->GetComponentUI()->ChangeColor(new_color);
	}

	fill_color = vec4(new_color.x, new_color.y, new_color.z, fill_color.w);
}

bool ComponentUI::Fade()
{
	for (auto it_go = my_go->children.begin(); it_go != my_go->children.end(); it_go++)
	{
		if ((*it_go)->GetComponentUI())
			(*it_go)->GetComponentUI()->Fade();
	}

	if (fill_color.w <= 0.05)
	{
		my_go->active = false;
		return true;
	}
	else
	{
		fill_color.w -= 0.5 * App->GetDT();
		return false;
	}

}

bool ComponentUI::CheckMouseInside(float2 mouse_pos)
{
	return (mouse_pos.x >= pos_x  && mouse_pos.x <= pos_x + width && mouse_pos.y >= pos_y && mouse_pos.y <= pos_y + height);
}

void UIPanel::GenerateBuffers()
{
	uint* index = new uint[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	//Cube Vertex
	glGenBuffers(1, &buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &buffer[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*6, index, GL_STATIC_DRAW);

	//UVs definition
	glGenBuffers(1, &buffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float2)*4, uv, GL_STATIC_DRAW);

}
