#include "ModuleFonts.h"


bool ModuleFonts::Init()
{
	LOG("Init True Type Font library");
	bool ret = true;


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
