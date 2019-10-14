#ifndef PANEL_H
#define PANEL_H

#include "Globals.h"
#include "imgui/imgui.h"

class Panel {
public:
	Panel() {};
	Panel(const char* n) :name(n) {};
	~Panel() {};

	virtual bool Start() { return true; };

	virtual update_status Draw() { return UPDATE_CONTINUE; };
	virtual bool IsEnabled() { return true; };
	virtual update_status Save() { return UPDATE_CONTINUE; };

protected:
	bool enabled;
	const char* name = "";
};


#endif // !PANEL_H

