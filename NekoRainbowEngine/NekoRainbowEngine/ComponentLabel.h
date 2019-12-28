#ifndef _COMPONENT_LABEL_H_
#define _COMPONENT_LABEL_H_

#include "ComponentUI.h"

class ComponentCharacter;
class ResourceMesh;
class Font;
class ResourceTexture;

class ComponentLabel :public ComponentUI {
public:
	ComponentLabel(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* text);
	~ComponentLabel();

private:
	void CreateText();

public:
	std::string text_str;
	std::vector<ComponentCharacter*> label_comp;

};


#endif // !_COMPONENT_LABEL_H_

