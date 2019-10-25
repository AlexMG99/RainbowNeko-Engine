#ifndef PANEL_TOPBAR_H_
#define PANEL_TOPBAR_H_

#include "Panel.h"

#include <map>
#include <list>

class PanelHelp;

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

};

#endif // !PANEL_TOPBAR_H_
