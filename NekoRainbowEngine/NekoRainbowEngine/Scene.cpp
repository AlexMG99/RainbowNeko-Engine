#include "Scene.h"
#include "Component.h"
#include "Parson/parson.h"

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
			//needs_removal = true;
		}
	}
}

Scene::~Scene()
{
	json_value_free(vroot);
}

bool Scene::AddUint(const char* field, uint value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

Scene Scene::GetSection(const char * name) const
{
	return Scene(json_object_get_object(root, name));
}
