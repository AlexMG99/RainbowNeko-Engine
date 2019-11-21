#ifndef PANEL_INSPECTOR_H_
#define PANEL_INSPECTOR_H_

#include "Panel.h"

class PanelInspector :public Panel {
public:
	PanelInspector() {};
	PanelInspector(const char* name) :Panel(name) {};
	~PanelInspector() {};

	update_status Draw();
};

#endif // !PANEL_INSPECTOR_H_
