#include "Application.h"
#include "ModuleViewport.h"
#include "PanelShape.h"

update_status PanelShape::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	static const char* combo_item[] = { "CUBE", "SPHERE", "CYLINDER", "CONE", "PLANE" };
	//Window Menu
	if (ImGui::Begin(name, &enabled))
	{
		ImGui::Text("Shapes"); ImGui::Separator();
		if (ImGui::Combo("Shape", &shape_t, combo_item, IM_ARRAYSIZE(combo_item)))
			App->viewport->CreateGameObjectShape(OBJECT_PARSHAPE, (shape_type)shape_t);

		ImGui::Separator();
		ImGui::End();
	}


	return ret;
}