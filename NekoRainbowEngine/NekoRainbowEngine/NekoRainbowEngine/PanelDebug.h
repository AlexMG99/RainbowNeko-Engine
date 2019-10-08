#ifndef PANEL_DEBUG_H_
#define PANEL_DEBUG_H_

#include "Panel.h"


class PanelDebug : public Panel {
public:
	PanelDebug() {};
	PanelDebug(const char* name) : Panel(name) {};
	~PanelDebug() {};

	bool Start();

	update_status Draw();

private:
	void Renderer();
	
private:
	bool open = false;


};





#endif
