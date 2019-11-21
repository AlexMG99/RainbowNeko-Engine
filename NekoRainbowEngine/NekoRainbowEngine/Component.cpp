#include "Component.h"
#include "GameObject.h"
#include "glmath.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Scene.h"
#include "MathGeoLib/include/MathGeoLib.h"

Component::Component(component_type comp_type, bool act, GameObject * obj)
{
	type = comp_type;
	active = act;
	my_go = obj;
}

Component::~Component()
{
	//RELEASE(my_go);
}

ComponentTransform::ComponentTransform(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj)
{
	local_position = position = { 0,0,0 };
	local_scale = scale = { 1,1,1 };
	local_rotation_euler = { 0,0,0 };
	local_rotation = rotation = Quat::identity;
	local_matrix = global_matrix = float4x4::identity;
}

bool ComponentTransform::ItIntersect(LineSegment ray)
{
	ComponentMesh* comp_mesh = my_go->GetComponentMesh();
	if (comp_mesh)
	{
		AABB inter_box = comp_mesh->BB_mesh.global_AABB;

		if (inter_box.IsFinite())
			return ray.Intersects(inter_box);
		else
			return false;
	}
	else
		return false;
		
}

bool ComponentTransform::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene trans_scene = scene.AddSectionArray(i);
	ret = trans_scene.AddInt("Type", type);
	ret = trans_scene.AddFloat3("Position", local_position);
	ret = trans_scene.AddFloat3("Rotation", local_rotation_euler);
	ret = trans_scene.AddFloat3("Scale", local_scale);

	return ret;
}

bool ComponentTransform::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene trans_scene = scene.GetSectionArray(i);

	type = (component_type)trans_scene.GetInt("Type");
	local_position = trans_scene.GetFloat3("Position");
	local_rotation_euler = trans_scene.GetFloat3("Rotation");
	local_scale = trans_scene.GetFloat3("Scale");

	return ret;
}

float4x4 ComponentTransform::GetGlobalTransformMatrix()
{
	local_rotation = Quat::FromEulerXYZ(local_rotation_euler.x * DEGTORAD, local_rotation_euler.y * DEGTORAD, local_rotation_euler.z * DEGTORAD);
	local_matrix = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (my_go->GetParent())
		global_matrix = my_go->GetParent()->GetComponentTransform()->GetGlobalTransformMatrix() * local_matrix;
	else
		global_matrix = local_matrix;

	global_matrix.Decompose(position, rotation, scale);

	return global_matrix;
}

void ComponentTransform::SetLocalTransform(float4x4 & t_matrix)
{
	t_matrix.Decompose(local_position, local_rotation, local_scale);
	local_rotation_euler = local_rotation.ToEulerXYZ();

	local_rotation_euler.x = RadToDeg(local_rotation_euler.x);
	local_rotation_euler.y = RadToDeg(local_rotation_euler.y);
	local_rotation_euler.z = RadToDeg(local_rotation_euler.z);

	GetGlobalTransformMatrix();

}

void ComponentTransform::CalculateGlobalAxis()
{
	float3x3 matrix = global_matrix.Float3x3Part();

	X = { 1,0,0 };
	Y = { 0,1,0 };
	Z = { 0,0,1 };

	X = matrix * X;
	Y = matrix * Y;
	Z = matrix * Z;
}

void ComponentTransform::SetGlobalAxis(float3 X, float3 Y, float3 Z)
{
	global_matrix.Set3x3Part(float3x3(X,Y,Z));
}

void ComponentTransform::UpdateComponents()
{
	for (auto it_child = my_go->children.begin(); it_child != my_go->children.end(); it_child++)
	{
		(*it_child)->GetComponentTransform()->UpdateComponents();
	}

	ComponentMesh* comp_mesh = my_go->GetComponentMesh();

	if (comp_mesh)
		comp_mesh->BB_mesh.UpdateBB(my_go);

	ComponentCamera* comp_camera = my_go->GetComponentCamera();
	if (comp_camera)
		comp_camera->UpdateFrustum(false);
}