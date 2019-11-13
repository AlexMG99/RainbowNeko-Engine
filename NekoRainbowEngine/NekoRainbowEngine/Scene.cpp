#include "Scene.h"
#include "Component.h"
#include "Parson/parson.h"

#include <string>

Scene::Scene()
{
	vroot = json_value_init_object();
	root = json_value_get_object(vroot);
}

Scene::Scene(const char* name)
{
	if (name != nullptr)
	{
		vroot = json_parse_string(name);
		if (vroot != nullptr) {
			root = json_value_get_object(vroot);
			json_object_set_value(root, name, json_value_init_object());
			scene_name = name;
		}
	}
}

Scene::Scene(JSON_Object* section) : root(section)
{
}

Scene::~Scene()
{
	json_value_free(vroot);
}

bool Scene::AddUint(const char* field, uint value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool Scene::AddFloat3(const char * field, float3 value)
{
	JSON_Value* va = json_value_init_array();
	array = json_value_get_array(va);
	json_object_set_value(root, field, va);

	json_array_append_number(array, value.x);
	json_array_append_number(array, value.y);
	json_array_append_number(array, value.z);

	return true;
}

bool Scene::AddQuat(const char * field, Quat value)
{
	JSON_Value* va = json_value_init_array();
	array = json_value_get_array(va);
	json_object_set_value(root, field, va);

	json_array_append_number(array, value.x);
	json_array_append_number(array, value.y);
	json_array_append_number(array, value.z);
	json_array_append_number(array, value.w);

	return true;
}

float3 Scene::GetFloat3(const char * field)
{
	float3 value;

	array = json_object_get_array(root, field);

	value.x = json_array_get_number(array, 1);
	value.y = json_array_get_number(array, 2);
	value.z = json_array_get_number(array, 3);

	return value;
}

Quat Scene::GetQuat(const char * field)
{
	Quat value;

	array = json_object_get_array(root, field);

	value.x = json_array_get_number(array, 1);
	value.y = json_array_get_number(array, 2);
	value.z = json_array_get_number(array, 3);
	value.w = json_array_get_number(array, 4);

	return value;
}

Scene Scene::GetSection(const char * name) const
{
	return Scene(json_object_get_object(root, name));
}

Scene Scene::AddSection(const char * name)
{
	json_object_set_value(root, name, json_value_init_object());
	scene_name = name;
	return GetSection(name);
}

bool Scene::Save(const char * name)
{
	json_serialize_to_file(vroot, name);
	return true;
}

std::string Scene::GetName() const
{
	return scene_name;
}

