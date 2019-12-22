#ifndef _RESOURCE_TEXTURE_H_
#define _RESOURCE_TEXTURE_H_

#include "Resource.h"
#include "Random.h"

class aiMaterial;

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	ResourceTexture(uint32 id);
	~ResourceTexture() {};

	Random Import(const aiMaterial* texture, const char* path);
	void Import(const aiMaterial * texture, const char * path, Random text_id);
	void GenerateTexture();
	bool Load();
	ResourceTexture* Load(Scene& scene);

	ResourceTexture* CreateTexture(const char* path);

public:
	uint width = 0, height = 0;
	uint image_id = 0;
	std::string path;
};

#endif // !_RESOURCE_TEXTURE_H_
