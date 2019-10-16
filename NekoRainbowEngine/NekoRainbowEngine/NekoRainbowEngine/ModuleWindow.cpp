#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui/imgui.h"

#include "Parson/parson.h"

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
		width = width * SCREEN_SIZE;
		height = height * SCREEN_SIZE;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		if(fullscreen_on == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable_on == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(border_on == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fullscreendesktop_on == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(project_name, 0, 35, width, height, flags);

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

bool ModuleWindow::Load()
{
	JSON_Object* obj = json_object(App->settings_doc);
	App->window->SetWindowSize(json_object_get_number(json_object_get_object(obj, "Application"), "Width"),
		json_object_get_number(json_object_get_object(obj, "Application"), "Height"));

	return true;
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
	return project_name;
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
	if (fullscreen_on)
		flags |= SDL_WINDOW_FULLSCREEN;
	else 
		flags = ~SDL_WINDOW_FULLSCREEN;

	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetResizable()
{
	SDL_SetWindowResizable(window, resizable_on ? SDL_TRUE:SDL_FALSE);
}

void ModuleWindow::SetBorderless()
{
	SDL_SetWindowBordered(window, border_on ? SDL_TRUE : SDL_FALSE);
}

void ModuleWindow::SetFullscreenDesktop()
{
	if (fullscreendesktop_on)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		flags = ~SDL_WINDOW_FULLSCREEN_DESKTOP;

	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetBrightness()
{
	SDL_SetWindowBrightness(window, brigthness);
}

void ModuleWindow::SetBrightness(float brightness)
{
	this->brigthness = brightness;
	//SDL_SetWindowBrightness(window, brigthness);
}

void ModuleWindow::SetWindowSize()
{
	SDL_SetWindowSize(window, width, height);
	App->renderer3D->OnResize(width, height);
}

void ModuleWindow::SetWindowSize(int width_, int height_)
{
	width = width_;
	height = height_;
	SDL_SetWindowSize(window, width, height);
	App->renderer3D->OnResize(width, height);
}