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
	glm::ivec2 getExtent() {
		return this->extent;
	}

	virtual void generateBuffer();
	void destroyBuffer();
	void generateBuffer(glm::vec3 center, glm::ivec2 screenSize, std::function<void(const RenderInfo &info)> renderMethod, RenderInfo info);
};

#endif /* cubeMapFramebufferTexture_h */
