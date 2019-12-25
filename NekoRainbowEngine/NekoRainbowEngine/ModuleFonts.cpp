#include "ModuleFonts.h"
#include "FreeType/include/ft2build.h"
#include "FreeType/include/freetype/freetype.h"

#pragma comment(lib, "FreeType/lib/freetype.lib")

bool ModuleFonts::Init()
{
	LOG("Init True Type Font library");
	bool ret = true;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		LOG("ERROR::FREETYPE: Could not init FreeType Library");

	return ret;
}

bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type library");
	RELEASE_ARRAY_LIST(fonts);
	return true;
}

Font * const ModuleFonts::LoadFont(const char * path, int size)
{
	Font* font = new Font;

	return font;
}

bool ModuleFonts::CalcSize(const char * text, int & width, int & height) const
{
	bool ret = false;


	return ret;
}
