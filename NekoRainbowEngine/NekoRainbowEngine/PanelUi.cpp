#include "Application.h"
#include "PanelUi.h"

#include <string>

#include "Brofiler/Brofiler.h"


bool PanelUi::Start()
{
	BROFILER_CATEGORY("Start_PanelUi", Profiler::Color::LimeGreen);

	return true;
}

update_status PanelUi::Draw() {
	BROFILER_CATEGORY("Draw_PanelUi", Profiler::Color::GoldenRod);


	//if (ImGui::BeginMenu(name))
	//{
	//	if (ImGui::MenuItem("Create a Button") && !create_button)
	//		create_button = true;
	//		
	//	if (ImGui::MenuItem("Create a Label") && !create_label);
	//	    create_label = true;
	//		
 //       if (ImGui::MenuItem("Create an Image") && !create_image);
	//		create_image = true;

	//    if (ImGui::MenuItem("Create a Checkbox") && !create_checkbox);
	//		create_checkbox = true;


	//	ImGui::EndMenu();
	//}

	//if (create_button) {
	//	CreateButton();
	//}
	/*else if (create_label) {
		CreateLabel();
	}*/
	/*else if (create_image) {
		CreateImage();
	}
	else if (create_checkbox) {
		CreateCheckBox();
	}*/
	

	return UPDATE_CONTINUE;
}


void PanelUi::CreateButton()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin("Button Panel", &create_button, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));
		/*ImGui::InputFloat2*/
		ImGui::Text("Decide what tipe of button you want to create using:");
	
		if (ImGui::Button("Create Button"))
			App->viewport->CreateUIElement("Button", UI_Button, 50, 50, App->viewport->canvas->GetComponentCanvas(), "auta", App->viewport->canvas, { 2,2,2 });
		
		
		ImGui::End();
	}
}

void PanelUi::CreateLabel()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin("Label Panel", &create_label, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));

		if (ImGui::Button("Create Label"))
			App->viewport->CreateUIElement("Label", UI_Label, 20, 50, App->viewport->canvas->GetComponentCanvas(), "auta", App->viewport->canvas, { 2,2,2 });
		
		ImGui::End();
	}
}

void PanelUi::CreateImage()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin("Label Panel", &create_image, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));

		if (ImGui::Button("Create Image"))
			App->viewport->CreateUIElement("Image", UI_Image, 20, 50, App->viewport->canvas->GetComponentCanvas(), "auta", App->viewport->canvas, { 2,2,2 });

		ImGui::End();
	}
}

void PanelUi::CreateCheckBox()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin("Label Panel", &create_checkbox, window_flags)) {
		ImGui::SetWindowSize(ImVec2(275, 300));

		if (ImGui::Button("Create Image"))
			App->viewport->CreateUIElement("Image", UI_Image, 20, 50, App->viewport->canvas->GetComponentCanvas(), "auta", App->viewport->canvas, { 2,2,2 });

		ImGui::End();
	}
}
