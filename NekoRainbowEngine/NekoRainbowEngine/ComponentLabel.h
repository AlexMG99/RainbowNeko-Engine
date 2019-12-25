#ifndef _COMPONENT_LABEL_H_
#define _COMPONENT_LABEL_H_

#include "ComponentUI.h"

struct SDL_Surface;
struct Texture;

class ResourceMesh;
class Font;
class ResourceTexture;

class ComponentLabel :public ComponentUI {
public:
	ComponentLabel(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* text);
	~ComponentLabel();

public:
	std::string text_str;

private:
	Font* text_font = nullptr;
	bool update_text = false;

};


#endif // !_COMPONENT_LABEL_H_

