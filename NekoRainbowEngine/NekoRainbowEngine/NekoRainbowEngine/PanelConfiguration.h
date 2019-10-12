#ifndef _PANEL_CONFIG_H_
#define _PANEL_CONFIG_H_

#include "Panel.h"
#include "PerfTimer.h"

#define MAX_HISTOGRAM_LOG 50

class PanelConfiguration : public Panel {
public:
	PanelConfiguration() {};
	PanelConfiguration(const char* name) : Panel(name) {};
	~PanelConfiguration();

	bool Start();
	update_status Draw();

	void AppWindow();

private:
	bool open = true;

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