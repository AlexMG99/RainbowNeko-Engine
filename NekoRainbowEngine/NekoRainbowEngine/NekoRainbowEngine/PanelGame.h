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
	bool visible = true;
};




#endif
