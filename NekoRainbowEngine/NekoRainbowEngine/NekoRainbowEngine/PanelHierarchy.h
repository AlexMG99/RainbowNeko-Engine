#ifndef _PANELHIERARCHY_H_
#define _PANELHIERARCHY_H_

#include "Panel.h"

class PanelHierarchy : public Panel {
public:
	PanelHierarchy() {};
	PanelHierarchy(const char* name) :Panel(name) {};
	~PanelHierarchy() {};

	update_status Draw();

private:

	bool visible = true;
};

#endif
