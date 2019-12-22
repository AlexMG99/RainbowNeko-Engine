#include "GameObject.h"
#include "Component.h"
#include "ComponentLabel.h"

ComponentLabel::ComponentLabel(component_type comp_type, bool act, GameObject * obj, UI_type type, uint w, uint h, ComponentCanvas * canvas): ComponentUI(comp_type, act, obj, type, w, h, canvas)
{
	ComponentTransform* comp_trans = my_go->GetComponentTransform();
	comp_trans->local_position = { comp_trans->local_position.x, comp_trans->local_position.y + canvas->height, comp_trans->local_position.z };
	comp_trans->GetGlobalTransformMatrix();
}
