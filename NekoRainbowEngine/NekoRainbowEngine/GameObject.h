#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <vector>

#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "Random.h"

class ComponentMesh;
class ComponentTexture;
class ComponentCamera;
class Scene;

enum object_type {
	OBJECT_NONE = -1,

	OBJECT_FBX,
	OBJECT_PARSHAPE
};

class GameObject {
public:
	GameObject();
	~GameObject();

	bool Update();

	//--------------- Components ---------------//
	Component* CreateComponent(component_type comp_type, bool active = true);

	bool SaveComponents(Scene scene);
	bool LoadComponents(Scene scene);

	ComponentTransform* GetComponentTransform();
	ComponentMesh* GetComponentMesh();
	ComponentTexture* GetComponentTexture();
	ComponentCamera* GetComponentCamera();

	void DeleteComponent(Component* comp);

	//--------------- Childrens ---------------//
	bool HasChildren() const;
	bool IsChild(GameObject* obj) const;
	bool IsDirectChild(GameObject * obj) const;
	void AddChild(GameObject* obj);
	void RemoveChild(GameObject* obj);

	void AddParent(GameObject* obj);
	GameObject* GetParent() const;
	void SetParent(GameObject * par);
	bool IsParentID(uint32 id);

	object_type GetType();
	void SetType(object_type ty_);

	uint32 GetId() const;
	void SetId();
	void SetId(double id);

	std::string GetName() const;
	void SetName(const char* name_);

	void SetSelected(bool select);
	float3 CorrectScale(const float3 scale) const;

	//Bounding Box
	void GenerateBoundingBuffers();
	void DrawBB();
	OBB GetOBB();
	AABB GetGlobalAABB();

	void UpdateBB();

	void CreateTransformAABB();

private:
	object_type type = OBJECT_NONE;
	std::string name = "Object Null";
	
	Random	id;
	GameObject*					parent = nullptr;
	std::vector<Component*>		components;

	uint id_vertexAABB = 0;
	uint id_vertexOBB = 0;
	uint id_indexBB = 0;

public:
	//AABB
	AABB local_AABB;
	AABB global_AABB;
	OBB global_OBB;
	std::vector<float3> vertices_AABB;
	std::vector<float3> vertices_OBB;
	std::vector<uint> index_BB;
	bool show_aabb = false;
	bool show_obb = false;

	ComponentTransform* transfrom;
	ComponentMesh *mesh;

	std::vector<GameObject*>	children;
	bool selected = false;
	bool active = true;

	uint32 parent_id;
};

#endif // !_GAMEOBJECT_H_

