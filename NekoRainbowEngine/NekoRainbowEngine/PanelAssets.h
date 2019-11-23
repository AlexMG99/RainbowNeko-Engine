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
	/*void DrawFolders(const Path& node);*/

	uint NodeTexture(const Path& node, uint64* item = nullptr, std::string* event = nullptr);



public:
	uint b_folder = 0;
	uint f_folder = 0;
};












#endif // _PANEL_ASSETS_H_
