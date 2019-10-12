#include "FBX.h"
#include "GL/include/glew.h"

//-------------- Devil --------------
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

bool FBX::LoadTextures(Mesh* mesh, const char * path)
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

		texture = new Texture();

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
			mesh->image_id = ilutGLBindTexImage();
			texture->height = ilGetInteger(IL_IMAGE_HEIGHT) / 2;
			texture->width = ilGetInteger(IL_IMAGE_WIDTH) / 2;

			texture->GenerateTexture(mesh->image_id);
		}

		free(Lump);
		ilDeleteImages(1, &devil_id);
	}
	else
	{
		LOG("The FBX doesn't have a texture path or path %s is incorrect", path);
		delete texture;
		texture = nullptr;
	}

	return ret;
}

Mesh::~Mesh()
{
	delete index;
	index = nullptr;

	delete vertices;
	vertices = nullptr;
}

void Mesh::GenerateMesh()
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

	LOG("Generated mesh with id vertex: %i and id index: %i", id_vertex, id_index);
}

void Mesh::Render()
{
	//Render FBX Mesh
	glEnableClientState(GL_VERTEX_ARRAY);
	if (UV_num > 0)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//UVs
	glEnable(GL_TEXTURE_2D);
 	if (UV_coord)
	{
		glBindTexture(GL_TEXTURE_2D, image_id);
		glBindBuffer(GL_ARRAY_BUFFER, uv_id);
		glTexCoordPointer(UV_num, GL_FLOAT, 0, (void*)0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	//Render Vertex Normals
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

FBX::~FBX()
{
	for (auto it_mesh = mesh_list.begin(); it_mesh != mesh_list.end(); ++it_mesh)
	{
		ilDeleteImages(1, &(*it_mesh)->image_id);
		delete (*it_mesh);
		(*it_mesh) = nullptr;
	}

	delete texture;
	texture = nullptr;
}

void Texture::GenerateTexture(uint& id)
{
	image_id = id;
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

void Texture::Render()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, image_id);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0, 0);
	glTexCoord2i(0, 1); glVertex2i(0, height);
	glTexCoord2i(1, 1); glVertex2i(width, height);
	glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}