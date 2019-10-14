#ifndef _PANELINSPECTOR_H_
#define _PNALEINSPECTOR_H_

#include "Panel.h"
#include "MathGeoLib/include/Math/float3.h"

class PanelInspector : public Panel {
public:
	PanelInspector() {};
	PanelInspector(const char* name) :Panel(name) {};
	~PanelInspector() {};

	update_status Draw();

private:
	float3 position;
};

#endif // !_PANELINSPECTOR_H_

