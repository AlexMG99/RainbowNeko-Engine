#include "Texture.h"
#include "GL/include/glew.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

void Texture::GenerateTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, image_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture()
{
	path = "";
}
