//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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
