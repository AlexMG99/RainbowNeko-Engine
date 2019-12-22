#ifndef _COMPONENT_UI_H_
#define _COMPONENT_UI_H_

#include "Component.h"
#include "ComponentCanvas.h"

enum UI_type {
	UI_None = -1,
	UI_Label,
	UI_Checkbox,
	UI_InpuText,
	UI_Button,
	UI_MAX

};

class ComponentUI :public Component {
public:
	ComponentUI(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas):Component(comp_type, act, obj)
	{
		this->canvas = canvas;
		this->type = type;
		height = h;
		width = w;
	};

	~ComponentUI() {};

	virtual bool OnHover() { return true; };
	virtual bool OnClicked() { return true; };
	virtual bool OnRelease() { return true; };
	virtual bool OnExit() { return true; };

	virtual void DebugDraw();
	virtual void Draw() {};
	void UpdateTransform();

	virtual bool Update() { return true; };

protected:
	bool dragable = false;
	bool clicked = false;

	uint width = 0, height = 0;
	UI_type type = UI_None;

	ComponentCanvas* canvas = nullptr;
};


#endif