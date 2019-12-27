#ifndef _COMPONENT_CHECKBOX_H_
#define _COMPONENT_CHECKBOX_H_

#include "ComponentUI.h"

class ResourceTexture;

class ComponentCheckbox: public ComponentUI
{
public:
	ComponentCheckbox(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path);
	~ComponentCheckbox() {};

	bool OnClick() override;

	void SetState(bool st);

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

private:
	bool bool_state = false;
	ResourceTexture* checkbox_true = nullptr;
};

#endif // !_COMPONENT_CHECKBOX_H_

