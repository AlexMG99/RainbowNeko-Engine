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
	ImVec2 GetWinSize() const;
	

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int width;
	int height;
};

#endif // __ModuleWindow_H__