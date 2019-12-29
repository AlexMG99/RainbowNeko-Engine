#ifndef _COMPONENT_IMAGE_H_
#define _COMPONENT_IMAGE_H_

#include "ComponentUI.h"

class ComponentImage:public ComponentUI
{
public:
	ComponentImage(component_type comp_type, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentImage() {};

	bool OnSave(Scene& scene, int i) const;
	bool OnLoad(Scene& scene, int i);

private:
	std::string img_path;
};

#endif // !_COMPONENT_IMAGE_H_

