#ifndef PANEL_UI_H
#define PANEL_UI_H

#include "Panel.h"


class PanelUi : public Panel {
public:
	PanelUi() {};
	PanelUi(const char* name) :Panel(name) {};
	~PanelUi() {};


	bool Start();

	update_status Draw();

	void CreateButton();
	void CreateLabel();

private:
	bool create_button = false;














};

#endif
