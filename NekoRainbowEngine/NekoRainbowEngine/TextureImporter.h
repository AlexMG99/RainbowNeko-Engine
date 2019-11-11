#ifndef _TEXTURE_IMPORTER_H_
#define _TEXTURE_IMPORTER_H_

#include "Importer.h"

class TextureImporter :public Importer {
public:
	TextureImporter() {};
	~TextureImporter() {};

	bool Init();

	bool ImportTexture(const char* path);
private:

};

#endif // !_TEXTURE_IMPORTER_H_

