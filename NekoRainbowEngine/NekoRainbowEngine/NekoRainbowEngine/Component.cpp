#include "Component.h"
#include "GameObject.h"
#include "glmath.h"

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
	local_position = { 0,0,0 };
	local_scale = { 1,1,1 };
	local_rotation_euler = { 0,0,0 };
	local_rotation = Quat::identity;
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
