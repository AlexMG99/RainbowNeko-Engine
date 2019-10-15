#include "FBO.h"
#include "Application.h"
#include "GL/include/glew.h"
#include "ModuleWindow.h"

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	//Create Depth attachement
	glGenTextures(1, &depth_id);
	glBindTexture(GL_TEXTURE_2D, depth_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	//Attach both textures to fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_id, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_id, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		LOG("Framebuffer created succesfully!");
		ret = false;
	}

	this->width = width;
	this->height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ret;
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glViewport(0, 0, width, height);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App->window->GetWinSize().x, App->window->GetWinSize().y);
}

void FBO::Delete()
{
	glDeleteFramebuffers(1, &fbo_id);
	glDeleteTextures(1, &color_id);
	glDeleteTextures(1, &depth_id);
}

GLuint FBO::GetTexture() const
{
	return fbo_id;
}
