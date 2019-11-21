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
	bool OnResize();
	
public:
	ImVec2 window_size;
	ImVec2 new_win_size;
	

	float WorldPosX = 0;
	float WorldPosY = 0;
	float width, height;
	float pos_y, pos_x;
};

#endif
