#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "Random.h"
#include <string>

typedef unsigned int uint;

enum resource_type
{
	RESOURCE_MESH,
	RESOURCE_MESH_UI,
	RESOURCE_TEXTURE_UI,
	RESOURCE_TEXTURE,
	RESOURCE_MODEL,
	RESOURCE_TOTAL
};

class Resource
{
	friend class ModuleResources;

public:
	Resource()
	{
		ID.GenerateRandomInt();
	}
	Resource(uint32 id, resource_type type)
	{
		if (id == 0)
			ID.GenerateRandomInt();
		else
			ID.SetNumber(id);
		this->type = type;
	}

	~Resource() {};

	virtual bool Load() { return true; };

	resource_type GetType() const
	{
		return type;
	}
	Random GetID() const
	{
		return ID;
	}
	const char* GetFile() const
	{
		return file.c_str();
	}
	const char* GetImportedFile() const
	{
		return imported_file.c_str();
	}
	uint CountReferences() const
	{
		return loaded;
	}

public:
	Random ID;
	std::string file = "";
	std::string imported_file = "";
	resource_type type;

	uint loaded = 0;

};


#endif // !_RESOURCE_H_

