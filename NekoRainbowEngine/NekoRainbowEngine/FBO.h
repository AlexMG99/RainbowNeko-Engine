#ifndef _FBO_H_
#define _FBO_H_

#include "Globals.h"
#include "glmath.h"
#include "imgui/imgui.h"

class ComponentCamera;

typedef unsigned int GLuint;

class FBO {
public:
	FBO() {};
	~FBO();

	bool Create(uint width, uint height);
	void Bind(ImVec2 size);
	void Unbind();
	void Delete();

	void PrepareModelView();
	void PrepareProjView();
	void PrepareOrthoView();
	void PrepareTexture();
	void PrepareDepth();

	GLuint GetTexture() const;
	ComponentCamera* GetComponentCamera() const;
	void SetComponentCamera(ComponentCamera* camera);

	ImVec2 GetTextureSize() const;

private:
	ComponentCamera* comp_camera = nullptr;

public:
	GLuint fbo_id = 0;
	GLuint color_id = 0;
	GLuint depth_id = 0;

	ImVec2 size;
	mat4x4 ProjectionMatrix;
};


#endif // !_FBO_H_

