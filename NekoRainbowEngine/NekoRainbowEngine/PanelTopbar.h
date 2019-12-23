#ifndef PANEL_TOPBAR_H_
#define PANEL_TOPBAR_H_

#include "Panel.h"

#include <map>
#include <list>

class PanelHelp;
class PanelUi;

class PanelTopbar :public Panel {
public:
	PanelTopbar() {};
	PanelTopbar(const char* name) :Panel(name) {};
	~PanelTopbar();

	bool Start();
	update_status Draw();
	void CreateStyle();

public:
	PanelHelp* panel_help = nullptr;
	PanelUi* panel_ui = nullptr;

};

#endif // !PANEL_TOPBAR_H_
