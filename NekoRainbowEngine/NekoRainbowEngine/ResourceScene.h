#ifndef _RESOURCE_SCENE_H_
#define _RESOURCE_SCENE_H_

#include "Resource.h"

class Scene;

class ResourceScene : public Resource
{
public:
	ResourceScene(Random id) :Resource() {};
	~ResourceScene() {};

private:
	Scene* scene = nullptr;
};

#endif // !_RESOURCE_SCENE_H_
