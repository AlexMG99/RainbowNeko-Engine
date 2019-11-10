#include "Application.h"
#include "ComponentCamera.h"
#include "Parson/parson.h"
#include "GL/include/glew.h"
#include <vector>

ComponentCamera::ComponentCamera(component_type comp_type, bool act, GameObject* obj): Component(comp_type, act, obj)
{
	if (my_go)
	{
		transform = my_go->GetComponentTransform();
		transform->position = float3::zero;
		transform->local_position = float3::zero;
	}
	c_frustum.type = FrustumType::PerspectiveFrustum;

	c_frustum.pos = float3::zero;
	c_frustum.front = float3::unitZ;
	c_frustum.up = float3::unitY;

	c_frustum.nearPlaneDistance = 10.0f;
	c_frustum.farPlaneDistance = 100.0f;
	c_frustum.verticalFov = 60.0f * DEGTORAD;
	c_frustum.horizontalFov = 2.0f * atanf(tanf(c_frustum.verticalFov / 2.0f) * 1.3f);

	ReloadFrustum();
	GenerateFrustumBuffers();
}

float* ComponentCamera::GetViewMatrix()
{
	vec3 X = { c_frustum.WorldRight().x, c_frustum.WorldRight().y, c_frustum.WorldRight().z };
	vec3 Y = { c_frustum.up.x, c_frustum.up.y, c_frustum.up.z };
	vec3 Z = { c_frustum.front.x, c_frustum.front.y, c_frustum.front.z };
	vec3 Position = { c_frustum.pos.x, c_frustum.pos.y, c_frustum.pos.z };

	mat4x4 ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);

	return &ViewMatrix;
}

bool ComponentCamera::Update()
{
	DrawFrustum();

	return true;
}

void ComponentCamera::ReloadFrustum()
{
	float3 corners[8];
	c_frustum.GetCornerPoints(corners);
	for (int i = 0; i < 8; i++)
	{
		vertices_frustum.push_back(corners[i]);
	}
	index_frustum = { 0,1, 0,4, 4,5, 5,1,
	3,2, 2,0, 0,1, 1,3,
	7,6, 6,2, 2,3, 3,7,
	6,4, 2,0,
	7,5, 3,1 };
}

void ComponentCamera::ChangePosition()
{
	transform->GetGlobalTransformMatrix();
	c_frustum.pos = transform->position;
	transform->CalculateGlobalAxis();
	c_frustum.front = transform->Z;
	c_frustum.up = transform->Y;
}

void ComponentCamera::GenerateFrustumBuffers()
{
	//Cube Vertex
	glGenBuffers(1, &id_vertices_frustum);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices_frustum);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices_frustum.size(), &vertices_frustum[0], GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &id_index_frustum);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index_frustum);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*index_frustum.size(), &index_frustum[0], GL_STATIC_DRAW);
}

void ComponentCamera::UpdateFrustum(bool camera)
{
	for (int i = 0; i < vertices_frustum.size();)
	{
		vertices_frustum.pop_back();
	}
	vertices_frustum.clear();

	if(!camera)
		ChangePosition();
	ReloadFrustum();

	glDeleteBuffers(1, &id_vertices_frustum);
	glDeleteBuffers(1, &id_index_frustum);

	GenerateFrustumBuffers();
}

void ComponentCamera::DrawFrustum()
{
	glColor3f(125, 125, 0);
	glLineWidth(2.0);

	glEnableClientState(GL_VERTEX_ARRAY);

	//Draw Global AABB
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices_frustum);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index_frustum);
	glDrawElements(GL_LINES, index_frustum.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glLineWidth(1);

	glBegin(GL_LINES);
	glVertex3f(vertices_frustum[0].x, vertices_frustum[0].y, vertices_frustum[0].z);
	glVertex3f(vertices_frustum[1].x, vertices_frustum[1].y, vertices_frustum[1].z);
	glEnd();

}

float3 ComponentCamera::GetCameraPosition() const
{
	return c_frustum.pos;
}

update_status ComponentCamera::Load()
{
	//BROFILER_CATEGORY("Load_ModuleCamera3D", Profiler::Color::LightGray);

	//JSON_Object* obj = json_object(App->settings_doc);
	//JSON_Object* cam_obj = json_object_get_object(json_object_get_object(obj, "Application"), "Camera");

	//base_speed = json_object_get_number(cam_obj, "Speed");
	//Position = { (float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "X"),
	//	(float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "Y"),
	//	(float)json_object_get_number(json_object_get_object(cam_obj, "Position"), "Z") };

	return UPDATE_CONTINUE;
}
