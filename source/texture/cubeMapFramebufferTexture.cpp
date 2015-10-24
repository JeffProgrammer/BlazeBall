//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//Most of this class has been adapted from the following site in some way or another.
//http://www.mbroecker.com/project_dynamic_cubemaps.html

#include "cubeMapFramebufferTexture.h"
#include "render/util.h"
#include <glm/gtc/matrix_transform.hpp>

CubeMapFramebufferTexture::~CubeMapFramebufferTexture() {
	destroyBuffer();
}

void checkStatus() {
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Status error: %08x\n", status);
}

void CubeMapFramebufferTexture::setExtent(const glm::ivec2 &extent) {
	//Hardcoded but very changeable. Please try to use powers of 2 and squares.
	this->extent = extent;

	//Cache the projection matrix for later
	projectionMat = glm::perspective(90.f, (F32)extent.x / (F32)extent.y, 0.1f, 500.f);

	destroyBuffer();
	generateBuffer();
}
void CubeMapFramebufferTexture::generateBuffer() {
	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &mBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mBuffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = PositiveX; i <= NegativeZ; i ++) {
		//Actually create the texture
		glTexImage2D(i, 0, GL_RGBA8, extent.x, extent.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}

	//Create a framebuffer for capturing the scene
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//Need a depth buffer so OpenGL knows what's in front. Otherwise the skybox
	// will render in front of everything else.
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, extent.x, extent.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Attach the depth buffer to the frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer);

	//Make sure we attach at least one color attachment so it completes the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, mBuffer, 0);
	checkStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// check for errors
	GL_CHECKERRORS();

	mGenerated = true;
}
void CubeMapFramebufferTexture::destroyBuffer() {
	if (!mGenerated)
		return;

	glDeleteFramebuffers(1, &framebuffer);
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteTextures(1, &mBuffer);
}
void CubeMapFramebufferTexture::generateBuffer(glm::vec3 center, glm::ivec2 screenSize, std::function<void(const RenderInfo &info)> renderMethod, RenderInfo info) {
	//Need to set viewport size otherwise it will only get the corner part of the frame
	glViewport(0, 0, extent.x, extent.y);

	static std::map<int, glm::mat4> renderDirections;
	if (renderDirections.size() == 0) {
		//Obtained by strict trial+error. Should be the 6 ortho directions, rotated correctly.
		renderDirections[PositiveX] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3( 1, 0, 0), glm::vec3(0, -1, 0));
		renderDirections[NegativeX] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
		renderDirections[PositiveY] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0,  1, 0), glm::vec3(0, 0,  1));
		renderDirections[NegativeY] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
		renderDirections[PositiveZ] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,  1), glm::vec3(0, -1, 0));
		renderDirections[NegativeZ] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

#ifdef GRAPHICS_DEBUG
	checkStatus();
#endif

	//We've copied info so we should set this
	info.isReflectionPass = true;

	//Render each of the faces of the cubemap
	for (int i = PositiveX; i <= NegativeZ; i ++) {
		//Set up the view matrix to orient around the given center point
		glm::mat4 viewMat = glm::translate(renderDirections[i], -center);

		//Make sure we draw to the correct texture
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, i, mBuffer, 0);

#ifdef GRAPHICS_DEBUG
		checkStatus();
		GL_CHECKERRORS();
#endif

		//Update the matrices on info (it's copied so this is ok)
		info.projectionMatrix = projectionMat;
		info.viewMatrix = viewMat;
		info.cameraPosition = center;

		//Let the scene render itself
		renderMethod(info);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, screenSize.x, screenSize.y);
}