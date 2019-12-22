#ifndef _MODULE_FONTS_H_
#define _MODULE_FONTS_H_

#include "Module.h"
#include <vector>

typedef struct _TTF_Font TTF_Font;

struct Font
{
	TTF_Font* font = nullptr;
	int size = 12;
};

class ModuleFonts :public Module
{
public:
	ModuleFonts(Application* app, bool start_enabled = true) : Module(app, start_enabled) {};
	~ModuleFonts() {};

	bool Init();
	bool CleanUp();

	// Load Font
	Font* const Load(const char* path, int size = 12);
	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:
	std::vector<Font*> fonts;
	Font* default_font = nullptr;

};


#endif // !_MODULE_FONTS_H_

