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

	void AppSettings();
	void WindowSettings();
	void InputSettings();
	void HardwareSettings();
	void RendererSettings();

private:

	float fps_log[MAX_HISTOGRAM_LOG];
	int fps_current_log = 0;
	float current_frames = 0;

	float ms_log[MAX_HISTOGRAM_LOG];
	int ms_current_log = 0;
	float current_ms = 0;
	int capped_fps = 60;
	int   win_w = 0;
	
	bool gl_depth;
	bool gl_cull_face;
	bool gl_lighting;
	bool gl_color_material;
	bool gl_texture_2d = false;
	bool gl_fog = false;
	bool gl_shade_model;
	bool gl_front_and_back = false;
	bool gl_points = false;
	bool gl_lines = false;
	bool gl_fill = true;

	int mode = 0;

	PerfTimer	check_time;
};


#endif // !PANEL_CONFIG_H_