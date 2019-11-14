#ifndef _SCENE_H_
#define _SCENE_H_

#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

class string;

class Scene
{
public:
	Scene();
	Scene(const char* name);
	Scene(JSON_Object* section);
	Scene(JSON_Object* section, JSON_Array* va);
	~Scene();

	bool AddUint(const char* field, uint value);
	bool AddInt(const char* field, int value);
	//bool AddFloat(const char* field, float value);
	bool AddFloat3(const char* field, float3 value);
	bool AddQuat(const char * field, Quat value);
	bool AddString(const char * field, std::string value);

	//bool GetInt(const char* field, int& value);
	float3 GetFloat3(const char* field);
	Quat GetQuat(const char * field);

	Scene GetSection(const char* name) const;
	Scene AddSection(const char* name);

	Scene AddSectionArray(int num);
	Scene AddArray(const char* name);

	bool Save(const char* name);

	std::string GetName() const;

private:
	JSON_Value* vroot = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;
	std::string scene_name;
};

#endif // !_SCENE_H_
