#ifndef _MODULE_FONTS_H_
#define _MODULE_FONTS_H_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float2.h"
#include <vector>
#include <map>

#define DEFAULT_FONT "./Fonts/Arial.ttf"

typedef char GLchar;

struct Character {
	uint    TextureID;  // ID handle of the glyph texture
	float2	Size;       // Size of glyph
	float2	Bearing;    // Offset from baseline to left/top of glyph
	uint    Advance;    // Offset to advance to next glyph
	unsigned char*	buffer;
};

struct Font {
	std::map<GLchar, Character> Characters;
	int size = 12;
};

class ModuleFonts :public Module
{
public:
	ModuleFonts(Application* app, bool start_enabled = true) : Module(app, start_enabled) {};
	~ModuleFonts() {};

	bool Init();
	bool Start();
	bool CleanUp();

	// Load Font
	Font* const LoadFont(const char* path, int size = 12);
	bool CalcSize(const char* text, int& width, int& height) const;

public:
	std::vector<Font*> fonts;
	Font* default_font = nullptr;

};


#endif // !_MODULE_FONTS_H_

