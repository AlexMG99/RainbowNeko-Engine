#ifndef _PANEL_IMPORTER_H_
#define _PANEL_IMPORTER_H_

#include "Panel.h"

class PanelImporter : public Panel {
public: 
	PanelImporter() {};
	PanelImporter(const char* name) :Panel(name) {};
	~PanelImporter() {};


	update_status Draw();


};












#endif // !PANEL_IMPORTER_H_
