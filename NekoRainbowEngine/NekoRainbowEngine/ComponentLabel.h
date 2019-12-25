#ifndef _COMPONENT_LABEL_H_
#define _COMPONENT_LABEL_H_

#include "ComponentUI.h"

struct SDL_Surface;
struct Texture;

class ResourceMesh;
class ResourceTexture;
class SDL_Surface;

class ComponentLabel :public ComponentUI {
public:
	ComponentLabel(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas);
	~ComponentLabel();

private:
	bool CreateText();
	void UpdateText();

	bool Update();
	void Draw();



public:
	std::string text_str;

	ResourceMesh* mesh = nullptr;
	ResourceTexture* texture = nullptr;

private:
	bool update_text = false;

};


#endif // !_COMPONENT_LABEL_H_

