#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	Cube aleron(info.aleron_size.x, info.aleron_size.y, info.aleron_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aleron.transform);
	left_light.size = { info.left_light.x, info.left_light.y, info.left_light.z };
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&left_light.transform);
	right_light.size = { info.right_light.x, info.right_light.y, info.right_light.z };
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&right_light.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	btVector3 aleron_offset(info.aleron_offset.x, info.aleron_offset.y, info.aleron_offset.z);
	aleron_offset = aleron_offset.rotate(q.getAxis(), q.getAngle());
	btVector3 left_light_offset(info.left_light_offset.x, info.left_light_offset.y, info.left_light_offset.z);
	left_light_offset = left_light_offset.rotate(q.getAxis(), q.getAngle());
	btVector3 right_light_offset(info.right_light_offset.x, info.right_light_offset.y, info.right_light_offset.z);
	right_light_offset = right_light_offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color = Pink;

	aleron.transform.M[12] += aleron_offset.getX();
	aleron.transform.M[13] += aleron_offset.getY();
	aleron.transform.M[14] += aleron_offset.getZ();
	aleron.color = Pink;

	left_light.transform.M[12] += left_light_offset.getX();
	left_light.transform.M[13] += left_light_offset.getY();
	left_light.transform.M[14] += left_light_offset.getZ();

	right_light.transform.M[12] += right_light_offset.getX();
	right_light.transform.M[13] += right_light_offset.getY();
	right_light.transform.M[14] += right_light_offset.getZ();

	aleron.Render();
	chassis.Render();
	left_light.Render();
	right_light.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

float PhysVehicle3D::Score() const
{
	return score;
}

vec3 PhysVehicle3D::GetForwardVector()
{
	btVector3 viewdirection;
	viewdirection = vehicle->getForwardVector();
	vec3 cardirection;
	cardirection.Set(viewdirection.x(), viewdirection.y(), viewdirection.z());

	return vec3();
}

void PhysVehicle3D::Orientation(float rot_angle)
{

	float matrix[16];
	memset(matrix, 0.0f, sizeof(matrix));

	// Keep position
	vec3 p = GetPos();
	matrix[12] = p.x;
	matrix[13] = p.y;
	matrix[14] = p.z;
	matrix[15] = 1;

	// Rotate the body by angle_r
	matrix[0] = cos(rot_angle);
	matrix[2] = -sin(rot_angle);
	matrix[5] = 1;
	matrix[8] = sin(rot_angle);
	matrix[10] = cos(rot_angle);

	SetTransform(matrix);

}

