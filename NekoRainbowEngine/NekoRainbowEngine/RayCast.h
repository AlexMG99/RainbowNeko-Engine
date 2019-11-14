#ifndef RAYCAST_H_
#define RAYCAST_H_
#include "Component.h"

struct RayCast
{
public:
	ComponentTransform* trans;
	float3 h_point;
	float distance;



	RayCast() : trans(nullptr), h_point(float3::zero), distance(0)
	{};
	RayCast(ComponentTransform* transform) : trans(transform), h_point(float3::zero), distance(0)
	{};



};





#endif // !RAYCAST_H_