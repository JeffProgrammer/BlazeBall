//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "framebufferTexture.h"
#include "render/util.h"
#include <glm/gtc/matrix_transform.hpp>

FramebufferTexture::~FramebufferTexture() {
	destroyBuffer();
}

void FramebufferTexture::setExtent(const glm::ivec2 &extent) {
	mExtent = extent;

	//Cache the projection matrix for later
	mProjectionMat = glm::perspective(glm::radians(90.f), (F32)extent.x / (F32)extent.y, 0.1f, 500.f);

	destroyBuffer();
	generateBuffer();
}
void FramebufferTexture::generateBuffer() {
	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &mColorBuffer);
	glBindTexture(GL_TEXTURE_2D, mColorBuffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Actually create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mExtent.x, mExtent.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GL_CHECKERRORS();

	//Generate the depth buffer
	glGenTextures(1, &mDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, mDepthBuffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Actually create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, mExtent.x, mExtent.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
	GL_CHECKERRORS();

	//Create a framebuffer for capturing the scene
	glGenFramebuffers(1, &mFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	GL_CHECKERRORS();

	//Need a render buffer so OpenGL knows what's in front. Otherwise the skybox
	// will render in front of everything else.
	glGenRenderbuffers(1, &mRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mExtent.x, mExtent.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	GL_CHECKERRORS();

	//Attach the depth buffer to the frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mFramebuffer);
	GL_CHECKERRORS();

	//Make sure we attach at least one color attachment so it completes the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);
	GL_CHECKERRORS();

#ifdef GRAPHICS_DEBUG
	GLL::checkFrameBufferStatus();
#endif

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// check for errors
	GL_CHECKERRORS();

	mGenerated = true;
}
void FramebufferTexture::destroyBuffer() {
	if (!mGenerated)
		return;

	glDeleteFramebuffers(1, &mFramebuffer);
	glDeleteRenderbuffers(1, &mRenderbuffer);
	glDeleteTextures(1, &mColorBuffer);
	glDeleteTextures(1, &mDepthBuffer);
}

void FramebufferTexture::activate(GLenum texNum) {
	//Error check
	if (!mGenerated)
		generateBuffer();

	//Activate and bind the buffer
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_2D, mColorBuffer);
	glActiveTexture(texNum + 1);
	glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
}

void FramebufferTexture::deactivate() {
	//Haha, this method is just BS. Fooled you.
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FramebufferTexture::generateBuffer(const RenderInfo &info) {
	if (!mGenerated) {
		generateBuffer();
	}

#ifdef GRAPHICS_DEBUG
	GLL::checkFrameBufferStatus();
#endif

	//We've copied info so we should set this
	RenderInfo framebufferInfo(info);

	framebufferInfo.viewport.size = mExtent;
	framebufferInfo.viewport.position = glm::ivec2(0, 0);
	framebufferInfo.pixelDensity = 1.0f;

	framebufferInfo.projectionMatrix = mProjectionMat;

	framebufferInfo.setViewport();
	glDisable(GL_SCISSOR_TEST);
	GL_CHECKERRORS();

	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	GL_CHECKERRORS();

	//Make sure we draw to the correct texture
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);

#ifdef GRAPHICS_DEBUG
	GLL::checkFrameBufferStatus();
	GL_CHECKERRORS();
#endif

	//Let the scene render itself
	framebufferInfo.renderWorld(framebufferInfo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_SCISSOR_TEST);
	info.setViewport();
	info.setScissor();
}