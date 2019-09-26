#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ImGui/imgui.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(fullscreen_on == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable_on == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}
	
	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
	
}
void ModuleWindow::SetOrganization(char * organ)
{
	organization = organ;
}
const char * ModuleWindow::GetTitle() const
{
	return name;
}
char * ModuleWindow::GetOrganization() const
{
	return organization;
}
ImVec2  ModuleWindow::GetWinSize() const {

	ImVec2 v(width, height);

	return v;
}

void ModuleWindow::SetFullscreen()
{
	if (fullscreen_on == true)
		flags |= SDL_WINDOW_FULLSCREEN;
	else 
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetResizable()
{
	SDL_SetWindowResizable(window, resizable_on ? SDL_TRUE:SDL_FALSE);
}