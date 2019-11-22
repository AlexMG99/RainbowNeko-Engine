#include "Application.h"
#include "PanelAssets.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler.h"
#include <string>
#include <stdio.h>
#include <map>
#include "imgui/imgui.h"

//PanelImporter::~PanelImporter()
//{
//}

update_status PanelAssets::Draw()
{

	BROFILER_CATEGORY("Draw_PanelAssets", Profiler::Color::GoldenRod);

	update_status ret = UPDATE_CONTINUE;
	ImGui::Begin(name, &enabled);

	/*GameObject* object = App->viewport->selected_object;

	if (object) 
	{
		ComponentTexture* comp_texture = object->GetComponentTexture();
		if (comp_texture) 
		{
		ImGui::Image((ImTextureID)comp_texture->image_id,ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}
	
	}*/
	
	/*for (std::map<std::string, ComponentTexture*>::iterator iter = )*/

	
		/*ImGui::Text("Assets"); ImGui::Separator();

		ImGui::Separator();*/
		ImGui::End();
	
	

	return ret;
}
