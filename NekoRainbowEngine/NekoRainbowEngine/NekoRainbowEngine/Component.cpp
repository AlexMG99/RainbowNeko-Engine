#include "Component.h"
#include "GameObject.h"

Component::~Component()
{
	delete my_go;
	my_go = nullptr;
}
