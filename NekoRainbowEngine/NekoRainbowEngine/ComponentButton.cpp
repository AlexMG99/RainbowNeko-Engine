#include "Application.h"
#include "ModuleResources.h"
#include "ComponentButton.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "GL/include/glew.h"

ComponentButton::ComponentButton(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas* canvas) :ComponentUI(comp_type, act, obj, type, w, h, canvas)
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	comp_trans->local_position = { comp_trans->local_position.x, comp_trans->local_position.y + canvas->height, comp_trans->local_position.z };
	comp_trans->GetGlobalTransformMatrix();

	ComponentMesh* comp_mesh = (ComponentMesh*)my_go->CreateComponent(COMPONENT_MESH);

	float3* vertex = new float3[4];
	vertex[0] = float3(0, 0, 0);
	vertex[1] = float3(width, 0, 0);
	vertex[2] = float3(0, height, 0);
	vertex[3] = float3(width, height, 0);

	mesh = new ResourceMesh();
	comp_mesh->AddMesh(mesh->CreateMesh(vertex));

	ComponentTexture* comp_text = (ComponentTexture*)my_go->CreateComponent(COMPONENT_TEXTURE);
	texture = (ResourceTexture*)(App->resources->CreateNewResource(resource_type::RESOURCE_TEXTURE));
	comp_text->AddTexture(texture->CreateTexture("./Assets/button.png"));
	comp_mesh->image_id = texture->image_id;

}

bool ComponentButton::Update()
{
	Draw();
	return true;
}

void ComponentButton::Draw()
{
	/*ComponentTransform* comp_trans = my_go->GetComponentTransform();
	ComponentTransform* comp_canvas_trans = canvas->my_go->GetComponentTransform();

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = comp_trans->position;

	float3 v1 = float3(pos.x, pos.y, pos.z);
	float3 v2 = float3(pos.x + width, pos.y, pos.z);
	float3 v3 = float3(pos.x, pos.y - height, pos.z);
	float3 v4 = float3(pos.x + width, pos.y - height, pos.z);

	glVertex3f(v3.x, v3.y, v3.z);
	glVertex3f(v4.x, v4.y, v4.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v1.x, v1.y, v1.z);

	glEnd();*/
}

