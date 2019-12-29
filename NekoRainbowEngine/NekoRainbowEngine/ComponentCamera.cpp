#include "Application.h"
#include "ComponentCamera.h"
#include "Parson/parson.h"
#include "GL/include/glew.h"
#include "Scene.h"
#include <vector>

ComponentCamera::ComponentCamera(component_type comp_type, bool act, GameObject* obj): Component(comp_type, act, obj)
{
	if (my_go)
	{
		transform = my_go->GetComponentTransform();
		transform->position = float3::zero;
		transform->local_position = float3::zero;
	}
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = 90.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

	Reference = float3(0, 0, 0);

	ReloadFrustum();
	GenerateFrustumBuffers();
}

ComponentCamera::ComponentCamera(component_type comp_type, bool act, GameObject * obj, float nP, float fP, float FOV) : Component(comp_type, act, obj)
{
	if (my_go)
	{
		transform = my_go->GetComponentTransform();
		transform->position = float3::zero;
		transform->local_position = float3::zero;
	}
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = nP;
	frustum.farPlaneDistance = fP;
	frustum.verticalFov = FOV * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

	Reference = float3(0, 0, 0);

	ReloadFrustum();
	GenerateFrustumBuffers();
}

ComponentCamera::~ComponentCamera()
{
	glDeleteBuffers(1, &id_vertices_frustum);
	glDeleteBuffers(1, &id_index_frustum);
}

float* ComponentCamera::GetViewMatrix()
{
	vec3 X = { frustum.WorldRight().x, frustum.WorldRight().y, frustum.WorldRight().z };
	vec3 Y = { frustum.up.x, frustum.up.y, frustum.up.z };
	vec3 Z = { -frustum.front.x, -frustum.front.y, -frustum.front.z };
	vec3 Position = { frustum.pos.x, frustum.pos.y, frustum.pos.z };

	mat4x4 ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);

	return &ViewMatrix;
}

bool ComponentCamera::Update()
{
	if(draw_frustrum)
		DrawFrustum();

	return true;
}

void ComponentCamera::ReloadFrustum()
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);
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
	frustum.pos = transform->position;
	transform->CalculateGlobalAxis();
	frustum.front = transform->Z;
	frustum.up = transform->Y;
}

void ComponentCamera::UpdateCameraPosition()
{
	transform->local_position = frustum.pos;
	transform->SetGlobalAxis(frustum.WorldRight(), frustum.up, frustum.front);
	transform->GetGlobalTransformMatrix();
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

	if (!camera)
		ChangePosition();
	else if(transform)
		UpdateCameraPosition();

	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

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

void ComponentCamera::Look(const float3 &position)
{
	float3 dir = position - frustum.pos;

	float3x3 m = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = m.MulDir(frustum.front).Normalized();
	frustum.up = m.MulDir(frustum.up).Normalized();
}

float3 ComponentCamera::GetCameraPosition() const
{
	return frustum.pos;
}

float4x4 ComponentCamera::GetOpenGLProjectionMatrix()
{
	return frustum.ProjectionMatrix().Transposed();
}

bool ComponentCamera::OnSave(Scene & scene, int i) const
{
	bool ret = false;
	Scene camera_scene = scene.AddSectionArray(i);

	ret = camera_scene.AddInt("Type", type);
	ret = camera_scene.AddFloat3("Position", frustum.pos);
	ret = camera_scene.AddFloat3("Front", frustum.front);
	ret = camera_scene.AddFloat3("Up", frustum.up);
	ret = camera_scene.AddFloat("NearPlane", frustum.nearPlaneDistance);
	ret = camera_scene.AddFloat("FarPlane", frustum.farPlaneDistance);
	ret = camera_scene.AddFloat("Fov", frustum.horizontalFov);

	return ret;
}

bool ComponentCamera::OnLoad(Scene & scene, int i)
{
	Scene camera_scene = scene.GetSectionArray(i);
	type = (component_type)camera_scene.GetInt("Type");
	frustum.pos = camera_scene.GetFloat3("Position");
	frustum.front = camera_scene.GetFloat3("Front");
	frustum.up = camera_scene.GetFloat3("Up");
	frustum.nearPlaneDistance = camera_scene.GetFloat("NearPlane");
	frustum.farPlaneDistance = camera_scene.GetFloat("FarPlane");
	frustum.horizontalFov = camera_scene.GetFloat("Fov");

	UpdateFrustum(true);

	return true;
}

bool ComponentCamera::ContainsAABox(const AABB& aabb) const
{
	float3 corners[8];
	int corner_in = 0;
	aabb.GetCornerPoints(corners); 

	for (int p = 0; p < 6; ++p) {
		int corner_in_count = 8;
		for (int i = 0; i < 8; ++i) {
			if (frustum.GetPlane(p).IsOnPositiveSide(corners[i])) {
				--corner_in_count;
			}
		}

		if (corner_in_count == 0)
			return false;
			
	}
	
	return true;
}