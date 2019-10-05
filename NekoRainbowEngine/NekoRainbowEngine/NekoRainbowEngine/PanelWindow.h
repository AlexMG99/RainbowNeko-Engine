#ifndef PANEL_EDIT_H_
#define PANEL_EDIT_H_

#include "Panel.h"

class PanelWindow : public Panel {
public:
	PanelWindow() {};
	PanelWindow(const char* name) :Panel(name) {};
	~PanelWindow() {};
	update_status Draw();
};


#endif // !PANEL_EDIT_H_
