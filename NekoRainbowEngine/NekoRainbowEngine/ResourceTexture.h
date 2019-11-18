#ifndef _RESOURCE_TEXTURE_H_
#define _RESOURCE_TEXTURE_H_

#include "Resource.h"
#include "Random.h"

class aiMaterial;

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	ResourceTexture(Random id) :Resource(id, type) {};
	~ResourceTexture() {};

	Random Import(const aiMaterial* texture, const char* path);
};

#endif // !_RESOURCE_TEXTURE_H_
