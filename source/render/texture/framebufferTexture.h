//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_TEXTURE_FRAMEBUFFERTEXTURE_H_
#define _RENDER_TEXTURE_FRAMEBUFFERTEXTURE_H_

#include "base/types.h"
#include "render/texture/texture.h"
#include "render/renderInfo.h"

class FramebufferTexture : public Texture {
protected:
	glm::ivec2 mExtent;
	glm::mat4 mProjectionMat;
	GLenum mFramebuffer;
	GLenum mRenderbuffer;
	
	GLenum mColorBuffer;
	GLenum mDepthBuffer;

	bool mGenerated;
public:
	FramebufferTexture(const glm::ivec2 &extent) : Texture() {
		mGenerated = false;
		setExtent(extent);
	}
	virtual ~FramebufferTexture();

	void setExtent(const glm::ivec2 &extent);

	inline glm::ivec2 getExtent() const {
		return mExtent;
	}

	virtual void activate(GLenum texNum);
	virtual void deactivate();
	
	virtual void generateBuffer();
	void destroyBuffer();
	void generateBuffer(const RenderInfo &info);
};

#endif
