#ifndef _FBO_H_
#define _FBO_H_

#include "Globals.h"

class ComponentCamera;
class ImVec2;

typedef unsigned int GLuint;

class FBO {
public:
	FBO() {};
	~FBO();

	bool Create(uint width, uint height);
	void Bind();
	void Unbind();
	void Delete();

	void PrepareModelView();
	void PrepareProjView();

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

	uint width, height;
};


#endif // !_FBO_H_

