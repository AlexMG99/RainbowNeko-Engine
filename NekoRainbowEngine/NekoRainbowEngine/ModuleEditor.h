#pragma once
#include "Module.h"
#include "Globals.h"
#include "Panel.h"
#include <list>

class PanelTopbar;
class PanelConfiguration;
class PanelConsole;
class PanelHierarchy;
class PanelInspector;
class PanelGame;
class PanelShape;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status Save();

	void DrawImGui();
	bool CleanUp();

private:
	void DrawPanels();

public:
	PanelTopbar*			panel_topbar = nullptr;
	PanelConfiguration*		panel_config = nullptr;
	PanelConsole*			panel_console = nullptr;
	PanelHierarchy*			panel_hierarchy = nullptr;
	PanelInspector*			panel_inspector = nullptr;
	PanelGame*				panel_game = nullptr;
	PanelShape*				panel_shape = nullptr;

};
