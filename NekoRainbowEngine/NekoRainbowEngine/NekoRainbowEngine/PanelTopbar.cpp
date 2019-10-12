#include "PanelTopbar.h"
#include "PanelConsole.h"
#include "PanelHelp.h"
#include "PanelConfiguration.h"

#include "Application.h"

bool PanelTopbar::Start()
{
	//Create Panels
	panel_topbar_map.insert(std::pair<std::string, Panel*>("Help", new PanelHelp("Help")));
	panel_map.insert(std::pair<std::string, Panel*>("Configuration", new PanelConfiguration("Configuration")));

	panel_console = new PanelConsole("Console");

	//Start other Panels
	for (auto it_panel_top = panel_topbar_map.begin(); it_panel_top != panel_topbar_map.end(); ++it_panel_top) {
		(*it_panel_top).second->Start();
	}

	for (auto it_panel = panel_map.begin(); it_panel != panel_map.end(); ++it_panel) {
		(*it_panel).second->Start();
	}

	panel_console->Start();

	return false;
}

update_status PanelTopbar::Draw()
{
	update_status ret = UPDATE_CONTINUE;
	//Draw Topbar Panel
	ImGui::BeginMainMenuBar();

	//File Menu
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit"))
			ret = UPDATE_STOP;

		ImGui::EndMenu();
	}

	//Help Menu
	for (auto it_panel_top = panel_topbar_map.begin(); it_panel_top != panel_topbar_map.end(); ++it_panel_top) {
		(*it_panel_top).second->Draw();
	}

	ImGui::EndMainMenuBar();

	for (auto it_panel = panel_map.begin(); it_panel != panel_map.end(); ++it_panel) {
		(*it_panel).second->Draw();
	}

	panel_console->Draw();

	return ret;
}
