//
//  cubeMapFramebufferTexture.hpp
//  DifViewerGame
//
//  Created by Glenn Smith on 10/21/15.
//
//

#ifndef cubeMapFramebufferTexture_h
#define cubeMapFramebufferTexture_h

#include "cubeMapTexture.h"
#include <functional>

class CubeMapFramebufferTexture : public CubeMapTexture {
protected:
	glm::ivec2 extent;
	GLenum framebuffer;
	GLenum depthBuffer;

public:
	CubeMapFramebufferTexture() : CubeMapTexture(std::vector<TextureInfo>()) {};

	virtual void generateBuffer();
	void generateBuffer(glm::vec3 center, std::function<void(glm::mat4)> renderMethod);
};

#endif /* cubeMapFramebufferTexture_h */
