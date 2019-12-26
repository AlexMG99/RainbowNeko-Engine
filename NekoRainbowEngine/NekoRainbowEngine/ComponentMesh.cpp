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
#include "PanelConsole.h"
#include "Scene.h"

#include "GL/include/glew.h"
#include <string>

ComponentMesh::ComponentMesh(component_type comp_type, bool act, GameObject * obj) : Component(comp_type, act, obj)
{
	wireframe_color = vec4(0, 1, 0, 1);
	vertex_color = vec4(1, 0, 0, 1);
	outline_color = vec4(0, 1, 1, 1);
	fill_color = vec4(1, 1, 1, 1);
	point_size = 1;
	line_width = 1;
	outline_width = 3;
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

void ComponentMesh::ChangeColor(vec4 color)
{
	fill_color = color;
}


void ComponentMesh::RenderFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(255, 255, 255, 1.0f);
	
	if (my_go->selected) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	//Render FBX Mesh
	glColor4f(fill_color.x, fill_color.y, fill_color.z, fill_color.w);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_VERTICES]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffers[BUFF_INDEX]);

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
	glDisable(GL_ALPHA_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Render Face Normals
	if (mesh->normals_face && normal_face_show) {
		glColor3f(1, 0.5, 0);
		glBegin(GL_LINES);
		for (int i = 0; i < mesh->norm_face_size; i += 2) {
			glVertex3f(mesh->normals_face[i].x, mesh->normals_face[i].y, mesh->normals_face[i].z);
			glVertex3f(mesh->normals_face[i].x + mesh->normals_face[i + 1].x, mesh->normals_face[i].y + mesh->normals_face[i + 1].y, mesh->normals_face[i].z + mesh->normals_face[i + 1].z);
		}

		glEnd();
	}

	if (mesh->normals_vertex && normal_show) {
		glColor3f(0, 1, 0.5);
		glBegin(GL_LINES);
		for (int i = 0; i < mesh->norm_vertex_size; i ++) {
			glVertex3f(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
			glVertex3f(mesh->vertices[i].x + mesh->normals_vertex[i].x, mesh->vertices[i].y + mesh->normals_vertex[i].y, mesh->vertices[i].z + mesh->normals_vertex[i].z);
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

bool ComponentMesh::DrawSelectedOutline()
{
	if (!glIsEnabled(GL_STENCIL_TEST))
		return false;

	glColor3f(outline_color.r, outline_color.g, outline_color.b);
	glLineWidth(outline_width);

	glStencilFunc(GL_NOTEQUAL, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glPolygonMode(GL_FRONT, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[BUFF_VERTICES]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffers[BUFF_INDEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, 0);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1);

	return true;
}

ComponentTexture::~ComponentTexture()
{
	RELEASE(texture);
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
