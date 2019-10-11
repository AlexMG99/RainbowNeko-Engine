#ifndef PANEL_DEBUG_H_
#define PANEL_DEBUG_H_

#include "Panel.h"
#include "PanelConsole.h"


class PanelDebug : public Panel {
public:
	PanelDebug() {};
	PanelDebug(const char* name) : Panel(name) {};
	~PanelDebug() {};

	bool Start();

	update_status Draw();
	
private:
	void RendererWindow();
	
private:
	bool open_renderer = false;

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

};





#endif
