#include "ModuleFonts.h"

#include "SDL\include\SDL.h"
#include "SDL2_ttf/include/SDL_ttf.h"
#pragma comment( lib, "SDL2_ttf/libx86/SDL2_ttf.lib" )

bool ModuleFonts::Init()
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = DEFAULT_FONT;
		int size = 12;
		default_font = LoadFont(path, size);
	}

	return ret;
}

bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type library");
	RELEASE_ARRAY_LIST(fonts);
	TTF_Quit();
	return true;
}

Font * const ModuleFonts::LoadFont(const char * path, int size)
{
	Font* font = new Font;
	font->font = TTF_OpenFont(path, size);
	font->size = size;

	if (font)
	{
		LOG("Font %s with size %i has been succesfully loaded!", path, size);
		fonts.push_back(font);
	}
	else
		LOG("Font with path %s NOT loaded, TTF Error: %s", path, TTF_GetError());

	return font;
}

bool ModuleFonts::CalcSize(const char * text, int & width, int & height, _TTF_Font * font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default_font->font, text, &width, &height) != 0) {
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		ret = true;
	}

	return ret;
}
