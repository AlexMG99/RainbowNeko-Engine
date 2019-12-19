#ifndef _COMPONENT_UI_H_
#define _COMPONENT_UI_H_

#include "Component.h"
#include "ComponentCanvas.h"

class ComponentUI :public Component {
public:
	ComponentUI():Component() {};
	~ComponentUI() {};

	virtual bool OnHover() {};
	virtual bool OnClicked() {};
	virtual bool OnRelease() {};
	virtual bool OnExit() {};

	virtual void DebugDraw() {};

private:
	bool dragable = false;
	bool clicked = false;

	ComponentCanvas* canvas = nullptr;
};


#endif