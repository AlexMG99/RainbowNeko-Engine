#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Globals.h"
#include <string>

class Texture
{
public:
	uint width = 0, height = 0;
	uint image_id = 0;
	std::string path;

	void GenerateTexture();

};


#endif // _TEXTURE_H_

