//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef cubeMapFramebufferTexture_h
#define cubeMapFramebufferTexture_h

#include "cubeMapTexture.h"
#include "render/renderInfo.h"
#include <functional>

class CubeMapFramebufferTexture : public CubeMapTexture {
protected:
	glm::ivec2 extent;
	glm::mat4 projectionMat;
	GLenum framebuffer;
	GLenum depthBuffer;

public:
	CubeMapFramebufferTexture(const glm::ivec2 &extent) : CubeMapTexture(std::vector<TextureInfo>()) {
		setExtent(extent);
	}
	virtual ~CubeMapFramebufferTexture();

	void setExtent(const glm::ivec2 &extent);

	inline glm::ivec2 getExtent() const {
		return this->extent;
	}

	virtual void generateBuffer();
	void destroyBuffer();
	void generateBuffer(const glm::vec3 &center, const glm::ivec2 &screenSize, std::function<void(RenderInfo &info)> renderMethod, RenderInfo info);
};

#endif /* cubeMapFramebufferTexture_h */
