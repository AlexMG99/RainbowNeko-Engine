#ifndef PANEL_CONFIG_H_
#define PANEL_CONFIG_H_

#include "Panel.h"

class PanelConfig: public Panel 
{
public:
	PanelConfig() {};
	PanelConfig(const char* name) :Panel(name) {};
	~PanelConfig() {};

	update_status Draw();
};



#endif // !PANEL_CONFIG_H_
