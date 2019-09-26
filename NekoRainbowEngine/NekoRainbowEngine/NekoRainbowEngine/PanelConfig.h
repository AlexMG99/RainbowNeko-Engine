#ifndef PANEL_CONFIG_H_
#define PANEL_CONFIG_H_

#include "Panel.h"
#include <vector>

class PanelConfig: public Panel 
{
public:
	PanelConfig() {};
	PanelConfig(const char* name) :Panel(name) {};
	~PanelConfig() {};

	update_status Draw();
private:
	void ConfigWindow();
private:
	bool open = false;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
};



#endif // !PANEL_CONFIG_H_
