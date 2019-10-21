#include "Brofiler/Brofiler.h"
#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"


update_status PanelHierarchy::Draw()
{

	BROFILER_CATEGORY("Draw_PanelHierarchy", Profiler::Color::GoldenRod);

	ImGui::Begin(name, &enabled);

	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		if(ImGui::Selectable((*it_obj)->GetName().c_str(), &(*it_obj)->selected))
		{
			App->viewport->CheckObjectSelected((*it_obj));
		}
	}

	ImGui::End();
	return UPDATE_CONTINUE;
}
