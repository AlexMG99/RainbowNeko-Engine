#ifndef _PANEL_ASSETS_H_
#define _PANEL_ASSETS_H_

#include "Panel.h"

#include "Path.h"

class PanelAssets : public Panel {
public: 
	PanelAssets() {};
	PanelAssets(const char* name) :Panel(name) {};
	~PanelAssets() {};


	update_status Draw();

private:
	void DrawFolders(const Path& node);


};












#endif // _PANEL_ASSETS_H_
