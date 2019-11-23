#include "Application.h"
#include "ModuleImporter.h"
#include "TextureImporter.h"
#include "Random.h"
#include "PanelAssets.h"
#include "ResourceTexture.h"
#include "ModuleResources.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler.h"
#include <string>
#include <map>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

//PanelImporter::~PanelImporter()
//{
//}

bool PanelAssets::Start()
{
	CreateNodeTexture("./Icons/Folder.png");
	CreateNodeTexture("./Icons/DDS.png");
	CreateNodeTexture("./Icons/Neko.png");
	CreateNodeTexture("./Icons/JPG.png");
	CreateNodeTexture("./Icons/Model.png");
	CreateNodeTexture("./Icons/PNG.png");
	CreateNodeTexture("./Icons/Scene.png");

	CreateNodes(nodes, LIBRARY_FOLDER);

	return true;
}

update_status PanelAssets::Draw()
{

	BROFILER_CATEGORY("Draw_PanelAssets", Profiler::Color::GoldenRod);

	update_status ret = UPDATE_CONTINUE;
	ImGui::Begin(name, &enabled);

	for (int i = 0; i < nodes.size(); i++)
	{
		actual_node->at(i).Draw(*actual_node);
	}

	ImGui::End();
	
	return ret;
}

void PanelAssets::CreateNodes(std::vector<Node>& node, const char* p)
{
	ResourceTexture* texture;
	std::vector<std::string> file_list, dir_list;
	App->fs->DiscoverFiles(p, file_list, dir_list);

	for (int i = 0; i < dir_list.size(); i++)
	{
		texture = (ResourceTexture*)App->resources->Get(node_textures.at("folder").GetNumber());
		std::string path = p + dir_list.at(i) + "/";
		node.push_back(Node(path.c_str(), dir_list.at(i).c_str(), texture->image_id, texture->width, texture->height));
	}

	for (int i = 0; i < file_list.size(); i++)
	{
		std::string extension;
		uint directory = 0;
		App->fs->SplitFilePath(file_list.at(i).c_str(), nullptr, nullptr, &extension);
		texture = (ResourceTexture*)App->resources->Get(node_textures.at(extension).GetNumber());

		if (extension == "neko")
			directory = 0;
		else if (extension == "model")
			directory = 1;
		else if (extension == "dds")
			directory = 3;

		std::string path = p + file_list.at(i);
		
		node.push_back(Node(path.c_str(), file_list.at(i).c_str(), texture->image_id, texture->width, texture->height));
	}

	for (auto it_child = node.begin(); it_child != node.end(); it_child++)
	{
		CreateNodes((*it_child).childrens, ((*it_child).path.c_str()));
	}
	
}

void PanelAssets::CreateNodeTexture(std::string path)
{
	Random ID = App->resources->ImportFile(path.c_str(), RESOURCE_TEXTURE);
	std::string file_name, extension;
	App->fs->SplitFilePath(path.c_str(), nullptr, &file_name, &extension);
	
	node_textures.insert(std::pair<std::string, Random>(file_name, ID));
}

uint PanelAssets::NodeTexture(const Path& node, uint64 * item, std::string * event)
{/*
	Random r_ID = 0;
	if (node.file == false)
	{
		if (event) event->assign("FOLDER");
		return folderBuffer;

	 }
	else
	{
		std::string m_files = node.path + (".meta");
		r_ID = App->resources->ImportFile(m_files.c_str(), RESOURCE_TEXTURE);
		

	}*/

	return uint();
}

void Node::Draw(std::vector<Node>& node)
{
	ImGui::BeginChild(local_path.c_str(), ImVec2(width*0.3, height*0.3));
	if (ImGui::ImageButton((ImTextureID)image_id, ImVec2(width * 0.2, height*0.2), ImVec2(0, 1), ImVec2(1, 0), 0))
	{
		node = childrens;
	}
	ImGui::Text("%s", local_path.c_str());
	ImGui::EndChild();
	ImGui::SameLine();
}
