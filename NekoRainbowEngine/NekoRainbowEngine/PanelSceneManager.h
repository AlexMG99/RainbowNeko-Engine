#ifndef _PANEL_SCENE_MANAGER_H_
#define _PANEL_SCENE_MANAGER_H_

#include "Panel.h"

class PanelSceneManager : public Panel {

public:

	PanelSceneManager():Panel() {};
	PanelSceneManager(const char* name) : Panel(name) {};
	~PanelSceneManager() {};

	update_status Draw();
	void ButtonFunctions();

};

#endif // !_PANEL_SCENE_MANAGER_H_

