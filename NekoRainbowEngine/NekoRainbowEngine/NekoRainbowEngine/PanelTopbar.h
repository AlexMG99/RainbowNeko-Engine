#ifndef PANEL_TOPBAR_H_
#define PANEL_TOPBAR_H_

#include "Panel.h"
#include "PanelConfiguration.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"
#include "PanelConsole.h"

#include <map>
#include <list>

class PanelTopbar :public Panel {
public:
	PanelTopbar() {};
	PanelTopbar(const char* name) :Panel(name) {};
	~PanelTopbar() {};

	bool Start();
	update_status Draw();
	void CreateStyle();
	void CreateDockLeft();
	void CreateDockRight();
	void CreateDockBottom();
	void CreateDockHier();


public:
	std::map< std::string, Panel*> panel_topbar_map;
	std::map< std::string, Panel*> panel_map;

	PanelConsole* panel_console = nullptr;
	PanelConfiguration* panel_configuration = nullptr;
	PanelInspector* panel_inspector = nullptr;
	PanelHierarchy* panel_hierarchy = nullptr;
};

#endif // !PANEL_TOPBAR_H_
