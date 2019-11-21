#ifndef PANEL_PLAY_H
#define PANEL_PLAY_H

#include "Panel.h"
#include "Timer.h"

class PanelPlay : public Panel {

public:

	PanelPlay();
	PanelPlay(const char* name) : Panel(name) {};
	~PanelPlay() {};

	update_status Draw();
	void ButtonFunctions();

private:
	Timer scene_timer;

};



#endif
