#ifndef PANEL_GAME_H_
#define PANEL_GAME_H_

#include "Panel.h"

class ImVec2;

class PanelGame : public Panel {
public:
	PanelGame() {};
	PanelGame(const char* name) : Panel(name) {};
	~PanelGame() {};
	
	update_status Draw();
	

public:
	ImVec2 size;

	float height = 742;
	float width = 1020;
	

	float WorldPosX = 0;
	float WorldPosY = 0;
};




#endif
