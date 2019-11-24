#ifndef _PANEL_ASSETS_H_
#define _PANEL_ASSETS_H_

#include "Panel.h"

#include "Path.h"

struct Node {

	friend class PanelAssets;
	
	Node() {};
	Node(const char* p, const char* lp, uint id, uint w, uint h, Node* par) :path(p), local_path(lp), image_id(id), width(w), height(h), parent(par) {};

	std::string path = "";
	std::string local_path = "";

	uint image_id = 0;
	uint width = 0;
	uint height = 0;

	void Draw(uint& num);

	Node* parent = nullptr;;
	std::vector<Node> childrens;

	bool draw = false;
};

class PanelAssets : public Panel {

	friend class Node;

public: 
	bool Start();

	PanelAssets() {};
	PanelAssets(const char* name) :Panel(name) {};
	~PanelAssets() {};


	update_status Draw();

	void ReloadPanel();

private:
	void CreateNodes(std::vector<Node>& node, const char* path, Node* parent);
	void CreateNodeTexture(std::string path);

public:
	std::map<std::string, Random> node_textures;

	std::vector<Node> nodes;

};












#endif // _PANEL_ASSETS_H_
