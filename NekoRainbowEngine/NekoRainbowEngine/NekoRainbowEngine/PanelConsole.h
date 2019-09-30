#ifndef PANEL_CONSOLE_H_
#define PANEL_CONSOLE_H_

#include "Panel.h"
#include <string>
#include "imgui/imgui.h"

class PanelConsole : public Panel {
public:
	PanelConsole() {};
	PanelConsole(const char* name) :Panel(name) {};
	~PanelConsole() {};

	bool Start();
	update_status Draw();

	void AddLog(const char* text);
	void Clear();
private:
	bool open = false;
	std::string console_text;

	ImGuiTextBuffer buffer_text;
};


#endif // !PANEL_CONSOLE_H_

