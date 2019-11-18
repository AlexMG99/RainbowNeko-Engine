#ifndef _TEXTURE_IMPORTER_H_
#define _TEXTURE_IMPORTER_H_

#include "Importer.h"

class ComponentTexture;
class Texture;

class TextureImporter :public Importer {
public:
	TextureImporter() {};
	~TextureImporter() {};

	bool Init();

	bool Import(const char* path, std::string output_file);
	bool ImportTexture(const char* path, std::string &output_file);
	Texture* Load(const char* file);


};

#endif // !_TEXTURE_IMPORTER_H_

