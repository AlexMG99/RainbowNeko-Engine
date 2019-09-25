#ifndef PANEL_EDIT_H_
#define PANEL_EDIT_H_

#include "Panel.h"

class PanelEdit : public Panel {
public:
	PanelEdit() {};
	PanelEdit(const char* name) :Panel(name) {};
	~PanelEdit() {};
	update_status Draw();
};


#endif // !PANEL_EDIT_H_
