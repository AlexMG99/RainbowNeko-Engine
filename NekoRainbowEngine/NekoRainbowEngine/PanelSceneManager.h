#ifndef _PANEL_SCENE_MANAGER_H_
#define _PANEL_SCENE_MANAGER_H_

#include "Panel.h"
#include "Timer.h"

class PanelSceneManager : public Panel {

public:

	PanelSceneManager();
	PanelSceneManager(const char* name) : Panel(name) {};
	~PanelSceneManager() {};

	update_status Draw();
	void ButtonFunctions();

private:
	Timer scene_timer;

};

#endif // !_PANEL_SCENE_MANAGER_H_

