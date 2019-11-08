#ifndef PANEL_HELP_H_
#define PANEL_HELP_H_

#include "Panel.h"

class PanelHelp: public Panel {
public:
	PanelHelp() {};
	PanelHelp(const char* name):Panel(name) {};
	~PanelHelp() {};

	bool Start();

	update_status Draw();

private:
	void RequestBrowser(const char* link);
	void AboutWindow();

private:
	//PanelHelp
	bool about_open = false;

	char documentation[70];
	char download_last[70];
	char report_bug[70];

	char engine_name[50];
	char authors[50];
	char description[50];
	char githublink[50];
	char SDL_link[50];
	char ImGui_link[50];
	char Glew_link[50];
	char OpenGL_link[50];
	char MathGeoLib_link[50];
	char PCG_link[50];
	char Parson_link[50];
	char Devil_link[50];
	char Assimp_link[50];
	char Par_Shapes_link[60];
	char Deviceld_link[70];
	char mmgr_link[70];
	char imguizmo_link[60];
	char license[2070];
};


#endif // !PANEL_HELP_H_
