#include "Globals.h"
#include "GL/include/glew.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui.h"
#include "ComponentCamera.h"

#include <gl/GL.h>

#include "Brofiler/Brofiler.h"

#pragma comment (lib, "GL/libx86/glew32.lib")    /* link glew Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	BROFILER_CATEGORY("Init_ModuleRenderer3D", Profiler::Color::Crimson);

	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync

		if (App->window->vsync_on && SDL_GL_SetSwapInterval(0) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		////Check for error
		GLenum error = glewInit();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing GLEW library! %s\n", SDL_GetError());
			ret = false;
		}
		else {
			LOG("Using Glew %s", glewGetString(GLEW_VERSION));
			LOG("Vendor: %s", glGetString(GL_VENDOR));
			LOG("Renderer: %s", glGetString(GL_RENDERER));
			LOG("OpenGL version supported %s", glGetString(GL_VERSION));
			LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	
	return ret;
}

bool ModuleRenderer3D::Start()
{
	// Projection matrix for
	OnResize(App->window->GetWinSize().x, App->window->GetWinSize().y);
	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_ModuleRenderer3D", Profiler::Color::SkyBlue);

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetSceneCamera()->GetCameraPosition().x, App->camera->GetSceneCamera()->GetCameraPosition().y, App->camera->GetSceneCamera()->GetCameraPosition().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	BROFILER_CATEGORY("PostUpdate_ModuleRenderer3D", Profiler::Color::DarkBlue);

	App->editor->DrawImGui();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)&App->camera->GetSceneCamera()->frustum.ProjectionMatrix().Transposed());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(App->camera->GetSceneCamera()->GetViewMatrix());
}

