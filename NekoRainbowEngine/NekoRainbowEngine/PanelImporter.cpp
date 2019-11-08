#include "Application.h"
#include "PanelImporter.h"
#include "SDL/include/SDL_opengl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler.h"
#include <string>
#include <stdio.h>
#include <map>

//PanelImporter::~PanelImporter()
//{
//}

update_status PanelImporter::Draw()
{

	BROFILER_CATEGORY("Draw_PanelImpotrter", Profiler::Color::GoldenRod);

	update_status ret = UPDATE_CONTINUE;
	ImGui::Begin(name, &enabled);
	
	/*for (std::map<std::string, ComponentTexture>)*/

	
		/*ImGui::Text("Assets"); ImGui::Separator();

		ImGui::Separator();*/
		ImGui::End();
	
	

	return ret;
}
