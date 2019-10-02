#include "Globals.h"
#include "GL/include/glew.h"

#include "Application.h"
#include "ModuleViewport.h"



ModuleViewport::ModuleViewport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleViewport::~ModuleViewport()
{}

bool ModuleViewport::Start()
{

	//Cube Vertex
	my_id = 0;
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
							//  0      1      2      3      4      5      6      7
	float vertices[8 * 3] = { 0,0,0, 1,0,0, 1,1,1, 1,1,0, 0,0,1, 0,1,1, 1,0,1, 0,1,0 };
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);

	//Cube Vertex definition
	my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
							 //    Back			  Right			Up			Left			Front			Down
	uint indices[12 * 3] = {3,1,0, 7,3,0, 2,6,1, 3,2,1, 2,3,7, 5,2,7, 5,0,4, 5,7,0, 2,5,4, 4,6,2, 0,1,6, 6,4,0 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);

	return true;
}

update_status ModuleViewport::PostUpdate(float dt)
{
	CreateGrid();

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
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
