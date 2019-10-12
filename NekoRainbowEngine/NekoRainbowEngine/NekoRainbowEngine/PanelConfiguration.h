#ifndef _PANEL_CONFIG_H_
#define _PANEL_CONFIG_H_

#include "Panel.h"



class PanelConfiguration : public Panel {
public:
	PanelConfiguration() {};
	PanelConfiguration(const char* name) : Panel(name) {};
	~PanelConfiguration();


	update_status Draw();

private:


	bool open = true;




};







#endif // !PANEL_CONFIG_H_