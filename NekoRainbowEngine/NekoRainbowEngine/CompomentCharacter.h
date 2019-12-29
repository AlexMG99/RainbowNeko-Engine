#ifndef _COMPONENT_CHARACTER_H_
#define _COMPONENT_CHARACTER_H_

#include "ComponentUI.h"

class ComponentCharacter:public ComponentUI
{
public:
	ComponentCharacter(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentCharacter() {};

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

private:
	std::string ch;
	Font* text_font = nullptr;
};

#endif
