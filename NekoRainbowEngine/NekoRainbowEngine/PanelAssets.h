#ifndef _PANEL_ASSETS_H_
#define _PANEL_ASSETS_H_

#include "Panel.h"

#include "Path.h"

struct Node {

	Node(const char* p, const char* lp, uint id, uint w, uint h) :path(p), local_path(lp), image_id(id), width(w), height(h) {};

	std::string path = "";
	std::string local_path = "";

	uint image_id = 0;
	uint width = 0;
	uint height = 0;

	void Draw();
};

class PanelAssets : public Panel {
public: 

	bool Start();

	PanelAssets() {};
	PanelAssets(const char* name) :Panel(name) {};
	~PanelAssets() {};


	update_status Draw();

private:
	void CreateNodes();
	void CreateNodeTexture(std::string path);
	/*void DrawFolders(const Path& node);*/

	uint NodeTexture(const Path& node, uint64* item = nullptr, std::string* event = nullptr);



public:
	uint b_folder = 0;
	uint f_folder = 0;

	std::map<std::string, Random> node_textures;

	std::vector<Node> nodes;

};












#endif // _PANEL_ASSETS_H_
