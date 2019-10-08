#include "Globals.h"
#include "GL/include/glew.h"

#include "Application.h"
#include "ModuleViewport.h"
#include "ModuleImporter.h"
#include "par/par_shapes.h"



ModuleViewport::ModuleViewport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleViewport::~ModuleViewport()
{}

bool ModuleViewport::Start()
{
	App->importer->CreateCube(0, 0, 0);
	return true;
}

update_status ModuleViewport::PostUpdate(float dt)
{
	CreateGrid();

	return UPDATE_CONTINUE;
}

bool ModuleViewport::CleanUp()
{
	return true;
}

void ModuleViewport::CreateGrid()
{
	glBegin(GL_LINES);
	for (int i = 0; i <= lines; i++) {
		glVertex3f(i*separation, 0, 0);
		glVertex3f(i*separation, 0, lines*separation);
		glVertex3f(0, 0, i*separation);
		glVertex3f(lines*separation, 0, i*separation);
	}
	glEnd();
}
