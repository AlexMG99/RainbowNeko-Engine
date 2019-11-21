#ifndef PANEL_SCENE_H_
#define PANEL_SCENE_H_

#include "Panel.h"

class ImVec2;

class PanelScene : public Panel {
public:
	PanelScene() {};
	PanelScene(const char* name) : Panel(name) {};
	~PanelScene() {};
	
	update_status Draw();
	
public:
	ImVec2 size;

	float height = 742;
	float width = 1020;

	float WorldPosX = 0;
	float WorldPosY = 0;
};

#endif
