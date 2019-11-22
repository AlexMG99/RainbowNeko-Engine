#ifndef _PANEL_ASSETS_H_
#define _PANEL_ASSETS_H_

#include "Panel.h"

class PanelAssets : public Panel {
public: 
	PanelAssets() {};
	PanelAssets(const char* name) :Panel(name) {};
	~PanelAssets() {};


	update_status Draw();


};












#endif // _PANEL_ASSETS_H_
