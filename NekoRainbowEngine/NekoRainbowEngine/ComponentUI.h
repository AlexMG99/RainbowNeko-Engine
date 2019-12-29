#ifndef _COMPONENT_UI_H_
#define _COMPONENT_UI_H_

#include "Component.h"
#include "ComponentCanvas.h"

class ResourceMesh;
class ResourceTexture;

struct UIPanel {
	float3 vertex[4];
	float2 uv[4];
	uint buffer[3];

	uint textureID;

	void GenerateBuffers();
};

enum UI_type {
	UI_None = -1,
	UI_Label,
	UI_Image,
	UI_Checkbox,
	UI_InpuText,
	UI_Character,
	UI_Button,
	UI_MAX

};

enum UI_state {
	UI_Idle,
	UI_Hover,
	UI_Click,
	UI_Clicked,
	UI_Release
};

class ComponentUI :public Component {
public:
	ComponentUI(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* str, uint x, uint y);

	~ComponentUI();

	virtual bool OnHover() { return true; };
	virtual bool OnClick() { return true; };
	virtual bool OnClicked();
	virtual bool OnRelease();

	virtual void Move();
	virtual void DebugDraw();
	virtual void Draw();
	void UpdateTransform();

	virtual bool Update();
	virtual bool UpdateUI(float dt) { return true; };
	void UILogic();

	bool Fade();

	virtual bool OnSave(Scene& scene, int i) const { return true; };
	virtual bool OnLoad(Scene& scene, int i) { return true; };

public:
	bool dragable = false;

private:
	bool CheckMouseInside(float2 mouse_pos);

protected:
	int pos_x = 0, pos_y = 0, width = 0, height = 0;
	UI_type ui_type = UI_None;
	UI_state state = UI_Idle;
	vec4 fill_color;

	UIPanel panel;
	ComponentCanvas* canvas = nullptr;

};


#endif