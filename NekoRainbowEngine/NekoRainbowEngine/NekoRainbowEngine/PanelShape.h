#ifndef PANEL_SHAPE_H_
#define PANEL_SHAPE_H_

#include "Panel.h"


class PanelShape :public Panel {
public:
	PanelShape() {};
	PanelShape(const char* name) :Panel(name) {};
	~PanelShape() {};

	update_status Draw();

private:
	int shape_t = -1;

};

#endif // !PANEL_SHAPE_H_
