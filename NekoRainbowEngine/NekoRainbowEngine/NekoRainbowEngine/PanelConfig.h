#ifndef PANEL_CONFIG_H_
#define PANEL_CONFIG_H_

#include "Panel.h"
#include "PerfTimer.h"
#include <array>

#define MAX_HISTOGRAM_LOG 25

class PanelConfig: public Panel 
{
public:
	PanelConfig() {};
	PanelConfig(const char* name) :Panel(name) {};
	~PanelConfig() {};

	bool Start();

	update_status Draw();
	update_status Save();

private:
	void ConfigWindow();

private:
	bool open = false;

	float fps_log[MAX_HISTOGRAM_LOG];
	int fps_current_log = 0;
	float current_frames = 0;

	float ms_log[MAX_HISTOGRAM_LOG];
	int ms_current_log = 0;
	float current_ms = 0;
	int capped_fps = 60;

	PerfTimer	check_time;
};



#endif // !PANEL_CONFIG_H_
