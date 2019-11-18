#ifndef _MODULE_RESOURCES_H_
#define _MODULE_RESOURCES_H_

#include "Module.h"
#include "Resource.h"
#include <map>

typedef unsigned __int32 uint32;
class Random;
class ResourceMesh;
class ResourceTexture;

class ModuleResources: public Module 
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool CleanUp();

	uint32 Find(const char* file);
	Resource* FindMeta(const char* file);
	Random ImportFile(const char* file_assets, resource_type type);
	Random GenerateNewID();
	const Resource* Get(Random id) const;
	Resource* Get(uint32 id);
	Resource* CreateNewResource(resource_type type);

	ResourceMesh* ImportMesh(uint32 id);
	ResourceTexture * ImportTexture(uint32 id, const char * path);

	void SaveMeta(const char* file, Resource* res);

public:
	std::map<uint32, Resource*> resources;
};


#endif // !_MODULE_RESOURCES_H_
