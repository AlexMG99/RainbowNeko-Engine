#ifndef _COMPONENT_CANVAS_H_
#define _COMPONENT_CANVAS_H_

#include "Component.h"

class GameObject;

class ComponentCanvas :public Component {
public:
	ComponentCanvas(component_type comp_type, bool act, GameObject* objs, uint w, uint h);
	~ComponentCanvas() {};

	bool Update();

	void DebugDraw();

public:
	uint width = 0, height = 0;
};


#endif // !_COMPONENT_CANVAS_H_
