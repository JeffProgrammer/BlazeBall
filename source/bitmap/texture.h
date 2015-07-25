//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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

#ifndef texture_h
#define texture_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif
#include <stdbool.h>
#include "base/types.h"

class Texture {
public:
	bool generated;
	GLuint buffer;
	GLenum texNum;
	glm::ivec2 extent;
	U8 *pixels;


	/**
	 Creates and allocates a Texture from a pixel array and extent
	 @arg pixels - A 4-component list of pixels in a RGBA list
	 @arg extent - A glm::ivec2 with the image extent
	 */
	Texture(U8 *pixels, glm::ivec2 extent);

	/**
	 Releases a Texture, freeing both its store and its buffer
	 */
	~Texture();

	void setTexNum(GLenum texNum) {
		this->texNum = texNum;
	}

	GLenum getTexNum() {
		return texNum;
	}

	/**
	 Generates the OpenGL buffer for a Texture. Don't call this before setting
	 up the OpenGL canvas!
	 */
	void generateBuffer();

	/**
	 Activates a Texture for drawing with OpenGL and binds its buffer
	 */
	void activate();

	/**
	 Deactivates a Texture after drawing
	 */
	void deactivate();

	/**
	 Find a texture in a directory
	 */
	static std::string find(const std::string &fullName);
};

#endif
