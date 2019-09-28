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