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
class PanelScene;
class PanelShape;
class PanelAssets;
class PanelPlay;
class PanelSceneManager;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status Save();

	void ChangeActualWindow(bool go);
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
	PanelScene*				panel_scene = nullptr;
	//PanelShape*				panel_shape = nullptr;
	PanelSceneManager*		panel_scene_manager = nullptr;
	PanelAssets*            panel_assets = nullptr;
	PanelPlay*              panel_play = nullptr;

public:
	bool gizmos = true;

};
