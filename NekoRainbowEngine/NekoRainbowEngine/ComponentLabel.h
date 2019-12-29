#ifndef _COMPONENT_LABEL_H_
#define _COMPONENT_LABEL_H_

#include "ComponentUI.h"

class ComponentCharacter;
class ResourceMesh;
class Font;
class ResourceTexture;

class ComponentLabel :public ComponentUI {
public:
	ComponentLabel(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* text, uint x, uint y);
	~ComponentLabel();

private:
	void CreateText();

	bool OnSave(Scene & scene, int i) const;

	bool OnLoad(Scene & scene, int i);

public:
	std::string text_str;

};


#endif // !_COMPONENT_LABEL_H_

