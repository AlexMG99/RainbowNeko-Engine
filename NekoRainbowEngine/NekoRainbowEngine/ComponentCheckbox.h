#ifndef _COMPONENT_CHECKBOX_H_
#define _COMPONENT_CHECKBOX_H_

#include "ComponentUI.h"

class ResourceTexture;

class ComponentCheckbox: public ComponentUI
{
public:
	ComponentCheckbox(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, bool state);
	~ComponentCheckbox() {};

private:
	bool* state;

	ResourceTexture* checkbox_true = nullptr;
};

#endif // !_COMPONENT_CHECKBOX_H_

