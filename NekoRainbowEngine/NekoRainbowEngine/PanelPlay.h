#ifndef PANEL_PLAY_H
#define PANEL_PLAY_H

#include "Panel.h"

#include "Timer.h"

class ImVec2;

class PanelPlay : public Panel {

public:

	PanelPlay();
	PanelPlay(const char* name) : Panel(name) {};
	~PanelPlay() {};

	update_status Draw();

public:
	ImVec2 window_size;
	ImVec2 cursor_pos;
	Timer scene_timer;

};

#endif
