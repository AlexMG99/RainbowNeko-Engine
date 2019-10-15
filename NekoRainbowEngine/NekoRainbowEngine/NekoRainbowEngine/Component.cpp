#include "Component.h"
#include "GameObject.h"

Component::Component(component_type comp_type, bool act, GameObject * obj)
{
	type = comp_type;
	active = act;
	my_go = obj;
}

Component::~Component()
{
	//RELEASE(my_go);
}
