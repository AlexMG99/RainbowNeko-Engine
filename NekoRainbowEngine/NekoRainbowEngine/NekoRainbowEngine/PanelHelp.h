#ifndef PANEL_HELP_H_
#define PANEL_HELP_H_

#include "Panel.h"

class PanelHelp: public Panel {
public:
	PanelHelp() {};
	PanelHelp(const char* name):Panel(name) {};
	~PanelHelp() {};
	update_status Draw();

	void RequestBrowser(const char* link);
};


#endif // !PANEL_HELP_H_
