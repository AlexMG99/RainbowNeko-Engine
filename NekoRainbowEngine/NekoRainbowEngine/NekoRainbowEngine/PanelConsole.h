#ifndef PANEL_CONSOLE_H_
#define PANEL_CONSOLE_H_

#include "Panel.h"
#include <string>
#include <list>
#include "imgui/imgui.h"

#define MAX_LINES 120

enum ConsoleTextType {
	CONSOLE_DEFAULT,
	CONSOLE_INFO,
	CONSOLE_SUCCESS,
	CONSOLE_WARNING,
	CONSOLE_ERROR
};

struct ConsoleText {
	ConsoleText(const char* txt, ConsoleTextType type_) :text(txt), type(type_) {};

	std::string text;
	ConsoleTextType type = ConsoleTextType::CONSOLE_DEFAULT;

};

class PanelConsole : public Panel {
public:
	PanelConsole() {};
	PanelConsole(const char* name) :Panel(name) {};
	~PanelConsole();

	update_status Draw();

	void AddLog(const char* text);
	void AddLog(ConsoleText text);

private:
	ImVec4 GetColorFromType(ConsoleTextType type);
	void CopyLogs();

private:
	std::list<ConsoleText> console_text;

public:
	bool start_console = false;
};


#endif // !PANEL_CONSOLE_H_

