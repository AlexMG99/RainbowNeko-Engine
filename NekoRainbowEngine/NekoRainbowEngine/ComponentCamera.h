#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_

#include "Component.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class vector;

class ComponentCamera: public Component
{
public:
	ComponentCamera(component_type comp_type, bool act, GameObject* obj);
	~ComponentCamera() {};

	bool Update();

	void ReloadFrustum();
	void ChangePosition();
	void GenerateFrustumBuffers();
	void UpdateFrustum(bool camera);
	void DrawFrustum();

	void Look(const float3 &position);

	float3 GetCameraPosition() const;
	float* GetViewMatrix();

	update_status Load();
	update_status Save() { return UPDATE_CONTINUE; };

public:
	Frustum frustum;

private:
	std::vector<float3> vertices_frustum;
	std::vector<uint> index_frustum;

	uint id_vertices_frustum, id_index_frustum;
	ComponentTransform* transform;
};

#endif // !_COMPONENT_CAMERA_H_

