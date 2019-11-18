#ifndef RAYCAST_H_
#define RAYCAST_H_
#include "Component.h"

struct RayCast
{
public:
	GameObject* go;
	float3 h_point;
	float distance;

	RayCast() : go(nullptr), h_point(float3::zero), distance(0)
	{};
	RayCast(GameObject* go_) : go(go_), h_point(float3::zero), distance(0)
	{};

};





#endif // !RAYCAST_H_