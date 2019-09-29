#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include <vector>
#include "ImGui/imgui.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetOrganization(char* organ);

	const char* GetTitle() const;
	char* GetOrganization() const;

	ImVec2 GetWinSize() const;

	//Change Window Properties
	void SetFullscreen();
	void SetResizable();
	void SetBorderless();
	void SetFullscreenDesktop();
	void SetBrightness();
	void SetBrightness(float brightness);
	void SetWindowSize();
	void SetWindowSize(int width_, int height_);
	

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

public:
	//PanelHelp
	char documentation[50];
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

	//PanelConfig
	char project_name[50] = "NekoRainbow Engine";
	char* organization = "UPC CITM";

	bool fullscreen_on = false;
	bool resizable_on = false;
	bool border_on = false;
	bool fullscreendesktop_on = false;

	float brigthness = 5.0f;
	int width;
	int height;
};

#endif // __ModuleWindow_H__