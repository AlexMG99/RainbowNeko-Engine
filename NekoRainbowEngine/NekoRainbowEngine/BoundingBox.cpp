#include "Application.h"
#include "ModuleViewport.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include "ResourceMesh.h"

#include "GL/include/glew.h"

//------------ Bounding Boxes -------------------------//
BoundingBox::BoundingBox()
{
	local_AABB.SetNegativeInfinity();

	index_BB = { 0,1, 0,4, 4,5, 5,1,
	3,2, 2,0, 0,1, 1,3,
	7,6, 6,2, 2,3, 3,7,
	6,4, 2,0,
	7,5, 3,1 };
}

BoundingBox::~BoundingBox()
{
	RELEASE_LIST(vertices_AABB);
	RELEASE_LIST(vertices_OBB);
	RELEASE_LIST(index_BB);
}

void BoundingBox::GenerateBoundingBuffers()
{
	//Global AABB vertices
	glGenBuffers(1, &id_vertexAABB);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertexAABB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices_AABB.size(), &vertices_AABB[0], GL_STATIC_DRAW);

	//Global OBB vertices
	glGenBuffers(1, &id_vertexOBB);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertexOBB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* vertices_OBB.size(), &vertices_OBB[0], GL_STATIC_DRAW);

	//Global BB vertices index
	glGenBuffers(1, &id_indexBB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexBB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* index_BB.size(), &index_BB[0], GL_STATIC_DRAW);
}


AABB BoundingBox::CreateLocalAABB(ResourceMesh* mesh)
{
	local_AABB.Enclose(mesh->vertices, mesh->vertices_size);

	return local_AABB;
}

OBB BoundingBox::GetOBB(GameObject* obj)
{
	global_OBB = local_AABB;
	global_OBB.Transform(obj->GetComponentTransform()->GetGlobalTransformMatrix());

	//Get Vertex and Index
	float3* aux_vertices = new float3[8];
	global_OBB.GetCornerPoints(aux_vertices);
	for (int i = 0; i < 8; i++)
	{
		vertices_OBB.push_back(aux_vertices[i]);
	}

	return global_OBB;
}

AABB BoundingBox::GetGlobalAABB(GameObject* obj)
{
	global_AABB.SetNegativeInfinity();
	global_AABB.Enclose(GetOBB(obj));

	//Get Vertex and Index
	float3* aux_vertices = new float3[8];
	global_AABB.GetCornerPoints(aux_vertices);
	for (int i = 0; i < 8; i++)
	{
		vertices_AABB.push_back(aux_vertices[i]);
	}

	return global_AABB;
}

void BoundingBox::DrawBB()
{
	glColor3f(1, 0, 0.2);
	glLineWidth(3.0);

	glEnableClientState(GL_VERTEX_ARRAY);

	//Draw Global AABB
	if (show_aabb)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_vertexAABB);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexBB);
		glDrawElements(GL_LINES, index_BB.size(), GL_UNSIGNED_INT, NULL);
	}

	//Draw OBB
	glColor3f(0.13, 0.59, 0.97);
	if (show_obb)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_vertexOBB);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexBB);
		glDrawElements(GL_LINES, index_BB.size(), GL_UNSIGNED_INT, NULL);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glLineWidth(1);
}

void BoundingBox::UpdateBB(GameObject* obj)
{
	CleanVertices();

	glDeleteBuffers(1, &id_vertexAABB);
	glDeleteBuffers(1, &id_indexBB);
	glDeleteBuffers(1, &id_vertexOBB);

	GetGlobalAABB(obj);

	GenerateBoundingBuffers();
}

void BoundingBox::CleanVertices()
{
	for (int i = 0; i < vertices_AABB.size();)
	{
		vertices_AABB.pop_back();
	}
	vertices_AABB.clear();

	for (int i = 0; i < vertices_OBB.size();)
	{
		vertices_OBB.pop_back();
	}
	vertices_OBB.clear();
}
