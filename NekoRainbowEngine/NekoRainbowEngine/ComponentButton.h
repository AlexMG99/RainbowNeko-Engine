#ifndef _COMPONENT_BUTTON_H_
#define _COMPONENT_BUTTON_H_

#include "ComponentUI.h"

class ComponentButton :public ComponentUI {
public:
	ComponentButton(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas);
	~ComponentButton() {};

	bool Update();

	void Draw();
};


#endif // !_COMPONENT_BUTTON_H_

