#ifndef _COMPONENT_UI_H_
#define _COMPONENT_UI_H_

#include "Component.h"
#include "ComponentCanvas.h"

class ResourceMesh;
class ResourceTexture;

enum UI_type {
	UI_None = -1,
	UI_Label,
	UI_Image,
	UI_Checkbox,
	UI_InpuText,
	UI_Button,
	UI_MAX

};

class ComponentUI :public Component {
public:
	ComponentUI(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* str);

	~ComponentUI() {};

	virtual bool OnHover();
	virtual bool OnClick();
	virtual bool OnRelease();
	virtual bool OnExit() { return true; };

	virtual void DebugDraw();
	virtual void Draw() {};
	void UpdateTransform();

	virtual bool Update();

protected:
	bool dragable = false;
	bool clicked = false;

	int pos_x = 0, pos_y = 0, width = 0, height = 0;
	UI_type type = UI_None;

	ResourceMesh* mesh = nullptr;
	ResourceTexture* texture = nullptr;
	ComponentCanvas* canvas = nullptr;

};


#endif