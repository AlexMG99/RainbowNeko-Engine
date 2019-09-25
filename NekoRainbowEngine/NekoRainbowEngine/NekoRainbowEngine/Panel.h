#ifndef PANEL_H
#define PANEL_H

#include "Globals.h"

class Panel {
public:
	Panel() {};
	Panel(const char* n) :name(n) {};
	~Panel() {};

	virtual update_status Draw() { return UPDATE_CONTINUE; };
	virtual bool IsEnabled() { return true; };

protected:
	bool enabled;
	const char* name = "";
};


#endif // !PANEL_H

