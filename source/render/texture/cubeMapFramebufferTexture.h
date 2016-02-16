//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_TEXTURE_CUBEMAPFRAMEBUFFERTEXTURE_H_
#define _RENDER_TEXTURE_CUBEMAPFRAMEBUFFERTEXTURE_H_

#include "cubeMapTexture.h"
#include "render/renderInfo.h"
#include <functional>

class CubeMapFramebufferTexture : public CubeMapTexture {
protected:
	glm::ivec2 mExtent;
	glm::mat4 mProjectionMat;
	GLenum mFramebuffer;
	GLenum mDepthBuffer;
public:
	CubeMapFramebufferTexture(const glm::ivec2 &extent) : CubeMapTexture(std::vector<TextureInfo>()) {
		mGenerated = false;
		setExtent(extent);
	}
	virtual ~CubeMapFramebufferTexture();

	void setExtent(const glm::ivec2 &extent);

	inline glm::ivec2 getExtent() const {
		return mExtent;
	}

	virtual void generateBuffer();
	void destroyBuffer();
	void generateBuffer(const glm::vec3 &center, const RenderInfo &info);
};

#endif // _RENDER_TEXTURE_CUBEMAPFRAMEBUFFERTEXTURE_H_