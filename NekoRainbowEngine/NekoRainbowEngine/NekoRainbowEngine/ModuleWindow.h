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

	void SetFullscreen();

	void SetResizable();
	

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	int width;
	int height;
	char* name = TITLE;
	char* organization = "UPC CITM";

public:
	bool fullscreen_on = false;
	bool resizable_on = false;
};

#endif // __ModuleWindow_H__