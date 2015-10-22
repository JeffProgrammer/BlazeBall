//
//  cubeMapFramebufferTexture.cpp
//  DifViewerGame
//
//  Created by Glenn Smith on 10/21/15.
//
//

#include "cubeMapFramebufferTexture.h"
#include "graphics/util.h"
#include <glm/gtc/matrix_transform.hpp>

void checkStatus() {
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Status error: %08x\n", status);
}

void CubeMapFramebufferTexture::generateBuffer() {
	extent = glm::ivec2(128, 128);

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

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, extent.x, extent.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, mBuffer, 0);
	checkStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// check for errors
	GL_CHECKERRORS();

	mGenerated = true;
}
void CubeMapFramebufferTexture::generateBuffer(glm::vec3 center, std::function<void(glm::mat4)> renderMethod) {
	glViewport(0, 0, extent.x, extent.y);
	static std::map<int, glm::mat4> renderDirections;
	if (renderDirections.size() == 0) {
		renderDirections[PositiveX] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3( 1, 0, 0), glm::vec3(0, -1, 0));
		renderDirections[NegativeX] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
		renderDirections[PositiveY] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0,  1, 0), glm::vec3(0, 0, 1));
		renderDirections[NegativeY] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
		renderDirections[PositiveZ] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,  1), glm::vec3(0, -1, 0));
		renderDirections[NegativeZ] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	checkStatus();

	for (int i = PositiveX; i <= NegativeZ; i ++) {
		glm::mat4 viewMat = renderDirections[i];
		viewMat = glm::translate(viewMat, -center);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, i, mBuffer, 0);
		checkStatus();

		GL_CHECKERRORS();

		renderMethod(viewMat);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}