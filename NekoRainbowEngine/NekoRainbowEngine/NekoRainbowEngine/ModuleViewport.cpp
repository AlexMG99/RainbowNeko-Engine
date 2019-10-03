#include "Globals.h"
#include "GL/include/glew.h"

#include "Application.h"
#include "ModuleViewport.h"
#include "par/par_shapes.h"



ModuleViewport::ModuleViewport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleViewport::~ModuleViewport()
{}

bool ModuleViewport::Start()
{
	cube = par_shapes_create_cube();

	//Cube Vertex
	my_id = 0;
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cube->npoints * 3, cube->points, GL_STATIC_DRAW);

	//Cube Vertex definition
	my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*cube->ntriangles * 3, cube->triangles, GL_STATIC_DRAW);

	return true;
}

update_status ModuleViewport::PostUpdate(float dt)
{
	CreateGrid();

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, cube->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

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
