#ifndef _COMPONENT_BUTTON_H_
#define _COMPONENT_BUTTON_H_

#include "ComponentUI.h"

class ResourceMesh;
class ResourceTexture;

class ComponentButton :public ComponentUI {
public:
	ComponentButton(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentButton() {};

	bool UpdateUI(float dt);
	bool OnHover() override;
	bool OnClick() override;

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

private:
	bool FadeToBlack(float dt);

	bool function = false;
};


#endif // !_COMPONENT_BUTTON_H_

