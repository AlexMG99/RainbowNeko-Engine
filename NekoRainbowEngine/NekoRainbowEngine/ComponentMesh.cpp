#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "PanelConfiguration.h"

#include "GL/include/glew.h"
#include <string>

//-------------- Devil --------------
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMesh::ComponentMesh(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj)
{
	wireframe_color = vec4(0, 255, 0, 1);
	vertex_color = vec4(255, 0, 0, 1);
	point_size = 1;
	line_width = 1;
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(UV_coord);
	RELEASE_ARRAY(normals);
	RELEASE_LIST(vertices);
	RELEASE_LIST(index);
	RELEASE_LIST(normals_face);

	RELEASE(transform);

	glDeleteBuffers(1, &id_vertex);
	glDeleteBuffers(1, &id_index);
	glDeleteBuffers(1, &uv_id);
	glDeleteBuffers(1, &normal_id);
}

bool ComponentMesh::Update()
{
	glPushMatrix();
	glMultMatrixf((float*)&transform->GetGlobalTransformMatrix().Transposed());

	if(App->viewport->fill_on)
		RenderFill();
	if(App->viewport->wireframe_on)
		RenderWireframe();
	if (App->viewport->point_on)
		RenderPoint();

	glPopMatrix();

	return true;
}

void ComponentMesh::GenerateMesh()
{
	//Cube Vertex
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*index.size(), &index[0], GL_STATIC_DRAW);

	//UVs definition
	if (UV_coord)
	{
		glGenBuffers(1, &uv_id);
		glBindBuffer(GL_ARRAY_BUFFER, uv_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*UV_num * vertices.size(), &UV_coord[0], GL_STATIC_DRAW);
	}

	//Normal Definition
	if (normals_face.size() > 0)
	{
		glGenBuffers(1, &normal_id);
		glBindBuffer(GL_ARRAY_BUFFER, normal_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * normals_face.size(), &normals[0], GL_STATIC_DRAW);
	}

	LOG("Generated mesh with id vertex: %i and id index: %i", id_vertex, id_index);
}

void ComponentMesh::RenderFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//Render FBX Mesh
	glColor3f(1, 1, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	//Normal
	if (normals_face.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, normal_id);
		glNormalPointer(GL_FLOAT, 0, nullptr);
	}

	//UVs
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

	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Render Vertex Normals
	if (normals && normal_show) {
		for (uint i = 0; i < vertices.size(); i++)
		{
			glColor3f(255, 0, 0);
			glBegin(GL_LINES);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			glVertex3f(vertices[i].x + normals[i].x, vertices[i].y + normals[i].y, vertices[i].z + normals[i].z);
			glEnd();
		}
	}

	//Render Face Normals
	if (normals_face.size() > 0 && normal_face_show) {
		glColor3f(0, 0, 255);
		glBegin(GL_LINES);
		for (int i = 0; i < vertices.size() - 1; i += 2) {
			glVertex3f(normals_face[i].x, normals_face[i].y, normals_face[i].z);
			glVertex3f(normals_face[i].x + normals_face[i + 1].x, normals_face[i].y + normals_face[i + 1].y, normals_face[i].z + normals_face[i + 1].z);
		}

		glEnd();
	}

}

void ComponentMesh::RenderWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(line_width);
	//Render FBX Mesh
	glColor3f(wireframe_color.x, wireframe_color.y, wireframe_color.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glLineWidth(1.0);
	glColor3f(1, 1, 1);
}

void ComponentMesh::RenderPoint()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	glEnable(GL_POINT_SMOOTH);
	glPointSize(point_size);
	//Render FBX Mesh
	glColor3f(vertex_color.x, vertex_color.y, vertex_color.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_POINT_SMOOTH);
	glPointSize(1.0f);

	glColor3f(1, 1, 1);
}

void ComponentTexture::LoadTexture(const char* path)
{
	bool ret = true;
	std::string file_path = ASSETS_FOLDER + std::string(path);

	if (path != "") {
		ILuint devil_id = 0;

		ilGenImages(1, &devil_id);
		ilBindImage(devil_id);
		ilutRenderer(ILUT_OPENGL);

		if (!ilLoadImage(file_path.c_str())) {
			auto error = ilGetError();
			LOG("Failed to load texture with path: %s. Error: %s", file_path.c_str(), ilGetString(error));
			ret = false;
		}
		else {
			image_id = ilutGLBindTexImage();
			height = ilGetInteger(IL_IMAGE_HEIGHT) / 2;
			width = ilGetInteger(IL_IMAGE_WIDTH) / 2;
		
			GenerateTexture();
			LOG("Loaded with path: %s succesfully!", file_path.c_str());
		}

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