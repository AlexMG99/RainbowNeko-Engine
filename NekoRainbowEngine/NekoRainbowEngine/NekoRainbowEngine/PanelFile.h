#ifndef PANEL_FILE_H_
#define PANEL_FILE_H_

#include "Panel.h"

class PanelFile : public Panel {
public:
	PanelFile() {};
	PanelFile(const char* name) :Panel(name) {};
	~PanelFile() {};
	update_status Draw();
};


#endif // !PANEL_FILE_H_
