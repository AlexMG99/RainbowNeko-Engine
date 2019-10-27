#ifndef _PANELHIERARCHY_H_
#define _PANELHIERARCHY_H_

#include "Panel.h"

#include <vector>

class PanelHierarchy : public Panel {
public:
	PanelHierarchy() {};
	PanelHierarchy(const char* name) :Panel(name) {};
	~PanelHierarchy() {};

	bool Start();
	update_status Draw();

	void TreeObject(GameObject* obj);

private:
	uint node_it = 0;
	uint node_max = 100;
	int object_id;
};

#endif
