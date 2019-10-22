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

	void ResizeWindow(ImVec2& start_s);

public:
	ImVec2 size;
};




#endif
