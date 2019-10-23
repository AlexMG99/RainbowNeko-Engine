#include "GameObject.h"
#include "ComponentMesh.h"

#include "GL/include/glew.h"

//-------------- Devil --------------
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(index);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(UV_coord);
	RELEASE_ARRAY(normals);
}

bool ComponentMesh::Update()
{
	Render();

	return true;
}

void ComponentMesh::GenerateMesh()
{
	//Cube Vertex
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num_vertices * 3, vertices, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_index, index, GL_STATIC_DRAW);

	//UVs definition
	if (UV_coord)
	{
		glGenBuffers(1, &uv_id);
		glBindBuffer(GL_ARRAY_BUFFER, uv_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*UV_num * num_vertices, &UV_coord[0], GL_STATIC_DRAW);
	}

	//Normal Definition
	/*if (normals_face.size() > 0)
	{
		glGenBuffers(1, &normal_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normal_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float3) *num_vertices, &normals[0], GL_STATIC_DRAW);
	}*/

	LOG("Generated mesh with id vertex: %i and id index: %i", id_vertex, id_index);
}

void ComponentMesh::Render()
{
	//Render FBX Mesh
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	//UVs
	glEnable(GL_TEXTURE_2D);
	if (UV_coord && my_go->GetComponentTexture())
	{
		if (my_go->GetComponentTexture()->active) 
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindTexture(GL_TEXTURE_2D, image_id);
			glBindBuffer(GL_ARRAY_BUFFER, uv_id);
			glTexCoordPointer(UV_num, GL_FLOAT, 0, (void*)0);
		}
	}

	//Checks if is a shape or a fbx
	if(par_shape)
		glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_SHORT, NULL);
	else
		glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	//Render Vertex Normals
	if (normals && normal_show) {
		uint j = 0;
		for (uint i = 0; i < num_vertices; i++)
		{
			glBegin(GL_LINES);
			glColor3f(255, 0, 0);

			glVertex3f(vertices[j], vertices[j + 1], vertices[j + 2]);

			glVertex3f(vertices[j] + normals[i].x, vertices[j + 1] + normals[i].y, vertices[j + 2] + normals[i].z);
			glEnd();
			j += 3;
		}
		glColor3f(1, 1, 1);
	}

	if (normals_face.size() > 0 && normal_face_show) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 255);
		for (int i = 0; i < (num_index / 3); i += 2) {
			glVertex3f(normals_face[i].x, normals_face[i].y, normals_face[i].z);
			glVertex3f(normals_face[i].x + normals_face[i + 1].x, normals_face[i].y + normals_face[i + 1].y, normals_face[i].z + normals_face[i + 1].z);
		}

		glEnd();
		glColor3f(1, 1, 1);
	}

}

void ComponentTexture::LoadTexture(const char* path)
{
	bool ret = true;

	if (path != "") {
		ILuint devil_id = 0;

		ilGenImages(1, &devil_id);
		ilBindImage(devil_id);
		ilutRenderer(ILUT_OPENGL);

		ILuint Size;
		FILE *File;
		ILubyte *Lump;

		File = fopen(path, "rb");
		fseek(File, 0, SEEK_END);
		Size = ftell(File);

		Lump = (ILubyte*)malloc(Size);
		fseek(File, 0, SEEK_SET);
		fread(Lump, 1, Size, File);
		fclose(File);

		if (!ilLoadL(IL_DDS, Lump, Size)) {
			auto error = ilGetError();
			LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
			ret = false;
		}
		else {
			image_id = ilutGLBindTexImage();
			height = ilGetInteger(IL_IMAGE_HEIGHT) / 2;
			width = ilGetInteger(IL_IMAGE_WIDTH) / 2;

			GenerateTexture();
		}

		free(Lump);
		ilDeleteImages(1, &devil_id);
		this->path = path;
	}
	else
	{
		LOG("The FBX doesn't have a texture path or path %s is incorrect", path);
	}
}

void ComponentTexture::GenerateTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, image_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

	glBindTexture(GL_TEXTURE_2D, 0);
}