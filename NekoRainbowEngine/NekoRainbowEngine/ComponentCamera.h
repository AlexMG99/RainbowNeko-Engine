#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_

#include "Component.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Geometry/AABB.h"

class vector;

class ComponentCamera: public Component
{
public:
	ComponentCamera(component_type comp_type, bool act, GameObject* obj);
	ComponentCamera(component_type comp_type, bool act, GameObject* obj, float nP, float fP, float FOV);
	~ComponentCamera() {};

	bool Update();

	void ReloadFrustum();
	void ChangePosition();
	void UpdateCameraPosition();
	void GenerateFrustumBuffers();
	void UpdateFrustum(bool camera);
	void DrawFrustum();

	void Look(const float3 &position);

	float3 GetCameraPosition() const;
	float* GetViewMatrix();

	float4x4 GetOpenGLProjectionMatrix();

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

	bool ContainsAABox(const AABB & aabb) const;

public:
	Frustum frustum;
	float3 Reference;
	bool update_proj = true;

private:
	std::vector<float3> vertices_frustum;
	std::vector<uint> index_frustum;

	uint id_vertices_frustum, id_index_frustum;
	ComponentTransform* transform = nullptr;
};

#endif // !_COMPONENT_CAMERA_H_

