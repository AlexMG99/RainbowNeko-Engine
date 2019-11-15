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
		vroot = json_parse_file(name);
		if (vroot != nullptr) {
			root = json_value_get_object(vroot);
			scene_name = name;
		}
	}
}

Scene::Scene(JSON_Object* section) : root(section)
{
}

Scene::Scene(JSON_Object* section, JSON_Array* va) : root(section), array(va)
{
}

Scene::~Scene()
{
	json_value_free(vroot);
}

bool Scene::AddInt(const char * field, int value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool Scene::AddDouble(const char * field, double value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool Scene::AddFloat3(const char * field, float3 value)
{
	JSON_Value* va = json_value_init_array();
	array = json_value_get_array(va);
	json_object_set_value(root, field, va);

	json_array_append_number(array, (float)value.x);
	json_array_append_number(array, (float)value.y);
	json_array_append_number(array, (float)value.z);

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

bool Scene::AddString(const char * field, std::string value)
{
	return json_object_set_string(root, field, value.c_str()) == JSONSuccess;
}

int Scene::GetInt(const char * field)
{
	return int(json_object_get_number(root, field));
}

double Scene::GetDouble(const char * field)
{
	return double(json_object_get_number(root, field));
}

float3 Scene::GetFloat3(const char * field)
{
	float3 value;

	aux_array = json_object_get_array(root, field);

	value.x = (float)json_array_get_number(aux_array, 0);
	value.y = (float)json_array_get_number(aux_array, 1);
	value.z = (float)json_array_get_number(aux_array, 2);

	return value;
}

Quat Scene::GetQuat(const char * field)
{
	Quat value;

	aux_array = json_object_get_array(root, field);

	value.x = json_array_get_number(aux_array, 0);
	value.y = json_array_get_number(aux_array, 1);
	value.z = json_array_get_number(aux_array, 2);
	value.w = json_array_get_number(aux_array, 3);

	return value;
}

std::string Scene::GetString(const char * field)
{
	return json_object_get_string(root, field);
}

bool Scene::IsString(const char * name) const
{
	return json_object_has_value_of_type(root, name, JSONString);
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

Scene Scene::AddSectionArray(int num)
{
	JSON_Value *leaf_value = json_value_init_object();
	JSON_Object* leaf_obj = json_value_get_object(leaf_value);
	json_array_append_value(array, leaf_value);

	return Scene(json_value_get_object(json_array_get_value(array, num)));
}

Scene Scene::GetSectionArray(int num)
{
	return Scene(json_value_get_object(json_array_get_value(array, num)), array);
}

bool Scene::IsArraySection(int num)
{
	bool ret = true;
	JSON_Value* obj = json_array_get_value(array, num);
	if (obj)
		ret = true;
	else
		ret = false;

	return ret;
}

Scene Scene::AddArray(const char* name)
{
	JSON_Value* va = json_value_init_array();
	array = json_value_get_array(va);
	json_object_set_value(root, name, va);

	return Scene(json_value_get_object(vroot), array);
}

Scene Scene::GetArray(const char * name)
{
	array = json_object_get_array(root, name);
	return Scene(json_value_get_object(vroot), array);
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

