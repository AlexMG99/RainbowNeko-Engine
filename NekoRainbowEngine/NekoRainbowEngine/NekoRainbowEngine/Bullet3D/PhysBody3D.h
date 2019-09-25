#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetLinearVelocity(float x, float y, float z);
	void DeleteBody();
	vec3 GetPos();

private:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__