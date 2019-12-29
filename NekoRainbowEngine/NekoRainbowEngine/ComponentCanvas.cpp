#include "Application.h"
#include "ComponentCanvas.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "PanelPlay.h"
#include "Scene.h"
#include "GL/include/glew.h"


ComponentCanvas::ComponentCanvas(component_type comp_type, bool act, GameObject * objs, uint w, uint h):Component(comp_type, act, objs)
{
	ComponentCamera* parent_cam = App->viewport->camera_game->GetComponentCamera();
	width = parent_cam->frustum.CornerPoint(3).x - parent_cam->frustum.CornerPoint(7).x;
	height = parent_cam->frustum.CornerPoint(7).y - parent_cam->frustum.CornerPoint(5).y;

	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	ComponentTransform* parent_trans = my_go->GetParent()->GetComponentTransform();
	
	comp_trans->local_position = float3(parent_cam->frustum.CornerPoint(5).x, parent_cam->frustum.CornerPoint(5).y, parent_cam->frustum.CornerPoint(5).z - 1);
	comp_trans->GetGlobalTransformMatrix();
}

bool ComponentCanvas::Update()
{
	DebugDraw();

	return true;
}

void ComponentCanvas::DebugDraw()
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

bool ComponentCanvas::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene canvas_scene = scene.AddSectionArray(i);

	ret = canvas_scene.AddInt("Type", type);
	ret = canvas_scene.AddInt("Width", width);
	ret = canvas_scene.AddInt("Height", height);

	return ret;
}

bool ComponentCanvas::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene canvas_scene = scene.AddSectionArray(i);

	type = (component_type)canvas_scene.GetInt("Type");
	width = canvas_scene.GetInt("Width");
	height = canvas_scene.GetInt("Height");

	return ret;
}


