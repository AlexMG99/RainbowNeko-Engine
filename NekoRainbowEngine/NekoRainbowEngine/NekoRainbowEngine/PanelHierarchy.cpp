#include "Application.h"
#include "ModuleViewport.h"
#include "PanelHierarchy.h"
#include "GameObject.h"

update_status PanelHierarchy::Draw()
{
	ImGui::Begin(name);
	for (auto it_obj = App->viewport->root_object->children.begin(); it_obj != App->viewport->root_object->children.end(); ++it_obj)
	{
		std::size_t pos = (*it_obj)->GetName().find_last_of("/");
		std::string str = (*it_obj)->GetName().substr(pos + 1);
		ImGui::Text(str.c_str());
	}

	ImGui::End();
	return UPDATE_CONTINUE;
}
