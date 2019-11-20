#ifndef _TEXTURE_IMPORTER_H_
#define _TEXTURE_IMPORTER_H_

#include "Importer.h"

class ComponentTexture;
class ResourceTexture;

class TextureImporter :public Importer {
public:
	TextureImporter() {};
	~TextureImporter() {};

	bool Init();

	bool Import(const char* path, std::string&  output_file);
	bool ImportTexture(const char* path, std::string &output_file);
	ResourceTexture* Load(const char* file);
	bool Load(ResourceTexture * texture);


};

#endif // !_TEXTURE_IMPORTER_H_

