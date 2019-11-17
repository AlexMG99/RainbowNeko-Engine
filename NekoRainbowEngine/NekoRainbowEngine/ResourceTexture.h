#ifndef _RESOURCE_TEXTURE_H_
#define _RESOURCE_TEXTURE_H_

#include "Resource.h"
#include "Random.h"

class aiMaterial;

class ResourceTexture : public Resource
{
public:
	ResourceTexture() :Resource() {};
	ResourceTexture(Random id) :Resource(id, type) {};
	~ResourceTexture() {};

	Random Import(const aiMaterial* texture, const char* path);
	//void ImportTexture(const char* path, std::string &output_file);
	//Texture* Load(const char* file);
};

#endif // !_RESOURCE_TEXTURE_H_
