#include "GameObject.h"
#include "Component.h"

GameObject::~GameObject()
{
	delete parent;
	parent = nullptr;

	for (auto it_obj = children.begin(); it_obj != children.end(); it_obj++) {
		delete (*it_obj);
		(*it_obj) = nullptr;
	}
	children.clear();

	for (auto it_comp = components.begin(); it_comp != components.end(); it_comp++) {
		delete (*it_comp);
		(*it_comp) = nullptr;
	}
	components.clear();
}
