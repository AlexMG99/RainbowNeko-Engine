#ifndef _COMPONENT_LABEL_H_
#define _COMPONENT_LABEL_H_

#include "ComponentUI.h"

struct SDL_Surface;
struct Texture;
struct Font;
typedef struct _TTF_Font TTF_Font;

class ComponentLabel :public ComponentUI {
public:
	ComponentLabel(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas);
	~ComponentLabel() {};



public:
	Font* font = nullptr;

};


#endif // !_COMPONENT_LABEL_H_

