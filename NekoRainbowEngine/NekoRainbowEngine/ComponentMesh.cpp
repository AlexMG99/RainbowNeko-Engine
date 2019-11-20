#include "Application.h"
#include "ModuleViewport.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "PanelConfiguration.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "Scene.h"

#include "GL/include/glew.h"
#include <string>

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
	if (App->viewport->camera_culling && !App->viewport->camera_game->GetComponentCamera()->ContainsAABox(BB_mesh.GetGlobalAABB(my_go)))
		return false;

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

	BB_mesh.DrawBB();

	return true;
}

bool ComponentMesh::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene mesh_scene = scene.AddSectionArray(i);

	mesh_scene.AddInt("Type", type);
	mesh_scene.AddString("Mesh", mesh->file.c_str());
	mesh_scene.AddDouble("Resource", mesh->ID.GetNumber());

	return ret;
}

bool ComponentMesh::OnLoad(Scene & scene, int i)
{
	Scene mesh_scene = scene.GetSectionArray(i);

	transform = my_go->GetComponentTransform();
	AddMesh(App->resources->ImportMesh(mesh_scene.GetDouble("Resource")));
	mesh->file = mesh_scene.GetString("Mesh");

	return true;
}

void ComponentMesh::AddMesh(ResourceMesh * mesh)
{
	this->mesh = mesh;
	transform = my_go->GetComponentTransform();
	BB_mesh.CreateLocalAABB(mesh);
	BB_mesh.global_AABB = BB_mesh.GetGlobalAABB(my_go);
	BB_mesh.GenerateBoundingBuffers();
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

bool ComponentTexture::OnSave(Scene & scene, int i) const
{
	bool ret = true;
	Scene texture_scene = scene.AddSectionArray(i);

	ret = texture_scene.AddInt("Type", type);
	ret = texture_scene.AddDouble("Resource", texture->GetID().GetNumber());

	return ret;
}

bool ComponentTexture::OnLoad(Scene & scene, int i)
{
	bool ret = true;
	Scene texture_scene = scene.AddSectionArray(i);

	type = (component_type)texture_scene.GetInt("Type");
	AddTexture(App->resources->ImportTexture(texture_scene.GetDouble("Resource")));

	my_go->GetComponentMesh()->image_id = texture->image_id;

	return ret;
}

void ComponentTexture::AddTexture(ResourceTexture* text)
{
	texture = text;
}
