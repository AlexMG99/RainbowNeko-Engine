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
{
	delete root_object;
	root_object = nullptr;
}

/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

bool ModuleViewport::Start()
{
	//App->importer->CreateCube(0, 0, 0);
	//App->importer->CreateCube(20, 0, 20);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);

	return true;
}



void ModuleViewport::makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}

update_status ModuleViewport::PostUpdate(float dt)
{
	CreateGrid(2,100);

	return UPDATE_CONTINUE;
}

bool ModuleViewport::CleanUp()
{

	return true;
}

void ModuleViewport::CreateGrid(uint separation, uint lines)
{

	//Vertex Point

	glPointSize(10.0f);

	glEnable(GL_POINT_SMOOTH);

	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();
	glDisable(GL_POINT_SMOOTH);

	glPointSize(1.0f);

	//Axis X,Y,Z
	glLineWidth(7.0);

	glBegin(GL_LINES);

	glColor3f(255, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1.5, 0, 0);

	glColor3f(0, 255, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1.5, 0);

	glColor3f(0, 0, 255);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1.5);

	glColor3f(255, 255, 255);
	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);
	for (int i = 0; i <= lines; i++) {
		glVertex3f(-((separation*lines)*0.5) + i*separation, 0, -((separation*lines)*0.5));
		glVertex3f(-((separation*lines)*0.5) +i*separation, 0, (separation*lines)*0.5);
		glVertex3f(-((separation*lines)*0.5), 0, -((separation*lines)*0.5) + i*separation);
		glVertex3f((separation*lines)*0.5, 0, -((separation*lines)*0.5) + i*separation);
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_TRIANGLES);

	float lenght = 1;
	//Down
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1* lenght, 0, 1 * lenght);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 1 * lenght);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 0, 1 * lenght);

	//Up
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 1 * lenght, 1 * lenght);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 0);

	//Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 1 * lenght);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 1 * lenght);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);

	//Left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);

	//Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, 1 * lenght);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1 * lenght, 0);

	//Back
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1 * lenght, 0, 1 * lenght);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glEnd();

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

}
