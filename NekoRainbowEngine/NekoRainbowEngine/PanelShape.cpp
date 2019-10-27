#include "Application.h"
#include "ModuleViewport.h"
#include "PanelShape.h"
#include "Brofiler/Brofiler.h"

update_status PanelShape::Draw()
{

	BROFILER_CATEGORY("Draw_PanelShape", Profiler::Color::GoldenRod);

	update_status ret = UPDATE_CONTINUE;

	static const char* combo_item[] = { "CUBE", "SPHERE", "CYLINDER", "CONE", "PLANE", "TORUS", "OCTAHEDRON" };
	//Window Menu
	if (ImGui::Begin(name, &enabled))
	{
		ImGui::Text("Shapes"); ImGui::Separator();
		if (ImGui::Combo("Shape", &shape_t, combo_item, IM_ARRAYSIZE(combo_item)))
			App->importer->CreateShape((shape_type)shape_t, 10, 10);

		ImGui::Separator();
		ImGui::End();
	}


	return ret;
}