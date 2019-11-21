#include "FBO.h"
#include "Application.h"
#include "GL/include/glew.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"

FBO::~FBO()
{
	Delete();
}

bool FBO::Create(uint width, uint height)
{
	bool ret = true;

	//Generate Buffers

	if (glGenFramebuffers == 0)
		LOG("Error, glGenFramebuffers not present!");
	glGenFramebuffers(1, &fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

	//Create Color attachement
	glGenTextures(1, &color_id);
	glBindTexture(GL_TEXTURE_2D, color_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_id, 0);

	//Create Depth attachement
	glGenRenderbuffers(1, &depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_id);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		LOG("Framebuffer created succesfully!");
		ret = false;
	}

	size = ImVec2(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ret;
}

void FBO::Bind(ImVec2 size_)
{
	size = size_;

	//Camera
	glViewport(0, 0, size.x, size.y);

	PrepareProjView();
	PrepareModelView();

	//Buffer Texture/Depth
	PrepareDepth();
	PrepareTexture();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FBO::Delete()
{
	glDeleteFramebuffers(1, &fbo_id);
	glDeleteTextures(1, &color_id);
	glDeleteTextures(1, &depth_id);
}

void FBO::PrepareModelView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(comp_camera->GetViewMatrix());
}

void FBO::PrepareProjView()
{

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		ProjectionMatrix = perspective(comp_camera->frustum.horizontalFov * RADTODEG, (float)size.x / (float)size.y, comp_camera->frustum.nearPlaneDistance, comp_camera->frustum.farPlaneDistance);
		glLoadMatrixf((float*)&ProjectionMatrix);
		
		comp_camera->update_proj = false;

}

void FBO::PrepareTexture()
{
	glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);

	//Reset buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FBO::PrepareDepth()
{
	glBindTexture(GL_TEXTURE_2D, color_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	//Reset buffer
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint FBO::GetTexture() const
{
	return fbo_id;
}

ComponentCamera * FBO::GetComponentCamera() const
{
	return comp_camera;
}

void FBO::SetComponentCamera(ComponentCamera* camera)
{
	comp_camera = camera;
}

ImVec2 FBO::GetTextureSize() const
{
	return size;
}
