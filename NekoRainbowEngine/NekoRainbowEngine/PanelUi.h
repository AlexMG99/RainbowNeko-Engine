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
	void CreateImage();
	void CreateCheckBox();


public:
	uint width;
	uint height;

	bool create_button = false;
	bool create_label = false;
	bool create_image = false;
	bool create_checkbox = false;














};

#endif
