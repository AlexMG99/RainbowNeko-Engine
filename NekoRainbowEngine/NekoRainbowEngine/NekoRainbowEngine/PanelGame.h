#ifndef PANEL_GAME_H_
#define PANEL_GAME_H_

#include "Panel.h"

class PanelGame : public Panel {
public:
	PanelGame() {};
	PanelGame(const char* name) : Panel(name) {};
	~PanelGame() {};

	update_status Draw();


};




#endif
