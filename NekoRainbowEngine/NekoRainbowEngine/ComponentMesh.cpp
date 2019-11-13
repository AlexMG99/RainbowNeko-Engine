#include "Application.h"
#include "ModuleViewport.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "PanelConfiguration.h"
#include "Mesh.h"

#include "GL/include/glew.h"
#include <string>

//-------------- Devil --------------
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMesh::ComponentMesh(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj)
{
	wireframe_color = vec4(0, 1, 0, 1);
	vertex_color = vec4(1, 0, 0, 1);
	outline_color = vec4(0.54, 0.16, 0.9, 0);
	point_size = 1;
	line_width = 1;
	outline_width = 7;
}

ComponentMesh::~ComponentMesh()
{
	RELEASE(transform);
	RELEASE(mesh);
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

	if (my_go->selected)
		DrawSelectedOutline();

	glPopMatrix();
	return true;
}

bool ComponentMesh::OnSave(Scene & scene) const
{
	return true;
}

void ComponentMesh::AddMesh(Mesh * mesh)
{
	this->mesh = mesh;
	CreateLocalAABB();
	my_go->global_OBB = GetOBB();
	my_go->global_AABB = GetGlobalAABB();
	my_go->GenerateBoundingBuffers();

}

//------------ Bounding Boxes -------------------------//
AABB ComponentMesh::CreateLocalAABB()
{
	my_go->local_AABB.Enclose(mesh->vertices, mesh->vertices_size);

	return my_go->local_AABB;
}

OBB ComponentMesh::GetOBB()
{
	OBB global_OBB = my_go->local_AABB;
	global_OBB.Transform(transform->GetGlobalTransformMatrix());

	//Get Vertex and Index
	float3* aux_vertices = new float3[8];
	global_OBB.GetCornerPoints(aux_vertices);
	for (int i = 0; i < 8; i++)
	{
		my_go->vertices_OBB.push_back(aux_vertices[i]);
	}

	return global_OBB;
}

AABB ComponentMesh::GetGlobalAABB()
{
	AABB global_AABB;
	global_AABB.SetNegativeInfinity();
	global_AABB.Enclose(GetOBB());

	//Get Vertex and Index
	float3* aux_vertices = new float3[8];
	global_AABB.GetCornerPoints(aux_vertices);
	for (int i = 0; i < 8; i++)
	{
		my_go->vertices_AABB.push_back(aux_vertices[i]);
	}
	my_go->index_BB = { 0,1, 0,4, 4,5, 5,1,	//Front
	3,2, 2,0, 0,1, 1,3,
	7,6, 6,2, 2,3, 3,7,
	6,4, 2,0,
	7,5, 3,1 };

	return global_AABB;
}

void ComponentMesh::RenderFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (my_go->selected) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	//Render FBX Mesh
	glColor3f(1, 1, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_VERTICES]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffers[BUFF_INDEX]);

	//Normal
	if (mesh->norm_face_size > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_NORMAL_FACE]);
		glNormalPointer(GL_FLOAT, 0, nullptr);
	}

	//UVs
	if (mesh->UV_size > 0)
	{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_UV]);
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);
	}

	if(my_go->GetComponentTexture())
		glBindTexture(GL_TEXTURE_2D, image_id);

	glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Render Vertex Normals
	//if (normals && normal_show) {
	//	for (uint i = 0; i < vertices_size; i++)
	//	{
	//		glColor3f(255, 0, 0);
	//		glBegin(GL_LINES);
	//		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	//		glVertex3f(vertices[i].x + normals[i].x, vertices[i].y + normals[i].y, vertices[i].z + normals[i].z);
	//		glEnd();
	//	}
	//}

	////Render Face Normals
	//if (normals_face.size() > 0 && normal_face_show) {
	//	glColor3f(0, 0, 255);
	//	glBegin(GL_LINES);
	//	for (int i = 0; i < normals_face.size(); i += 2) {
	//		glVertex3f(normals_face[i].x, normals_face[i].y, normals_face[i].z);
	//		glVertex3f(normals_face[i].x + normals_face[i + 1].x, normals_face[i].y + normals_face[i + 1].y, normals_face[i].z + normals_face[i + 1].z);
	//	}

	//	glEnd();
	//}

}

void ComponentMesh::RenderWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(line_width);
	//Render FBX Mesh
	glColor3f(wireframe_color.x, wireframe_color.y, wireframe_color.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_VERTICES]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffers[BUFF_INDEX]);

	glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, NULL);

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
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_VERTICES]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffers[BUFF_INDEX]);

	glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_POINT_SMOOTH);
	glPointSize(1.0f);

	glColor3f(1, 1, 1);
}

void ComponentMesh::DrawSelectedOutline()
{
	if (!glIsEnabled(GL_STENCIL_TEST))
		return;

	glColor3f(outline_color.r, outline_color.g, outline_color.b);
	glLineWidth(outline_width);

	glStencilFunc(GL_NOTEQUAL, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glPolygonMode(GL_FRONT, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_VERTICES]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffers[BUFF_INDEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, mesh->index_size * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1);

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