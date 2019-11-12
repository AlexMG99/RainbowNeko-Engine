#ifndef _SCENE_H_
#define _SCENE_H_

#include "Globals.h"
#include <string>
#include "MathGeoLib/include/Math/float3.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

class Scene
{
public:
	Scene();
	Scene(const char* name);
	Scene(JSON_Object* section);
	~Scene();

	bool AddUint(const char* field, uint value);
	bool AddFloat(const char* field, float value);
	bool AddFloat3(const char* field, float3 value);

	Scene GetSection(const char* name) const;
	Scene AddSection(const char* name);

	bool Save(const char* name);

private:
	JSON_Value* vroot = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;
	std::string scene_name;
};

#endif // !_SCENE_H_

