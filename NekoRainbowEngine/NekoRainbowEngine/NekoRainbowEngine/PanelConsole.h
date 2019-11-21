#ifndef PANEL_CONSOLE_H_
#define PANEL_CONSOLE_H_

#include "Panel.h"
#include <string>
#include "imgui/imgui.h"

class PanelConsole : public Panel {
public:
	PanelConsole() {};
	PanelConsole(const char* name) :Panel(name) {};
	~PanelConsole();

	update_status Draw();

	void AddLog(const char* text);
	void Clear();

private:
	bool open = true;
	std::string console_text;

	char console_input[50] = "";
	ImGuiTextBuffer buffer_text;
	bool scroll_to_bottom = false;
public:
	bool start_console = false;
};


#endif // !PANEL_CONSOLE_H_

