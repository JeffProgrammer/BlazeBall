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

#ifndef _RENDER_TEXTURE_BITMAPTEXTURE_H_
#define _RENDER_TEXTURE_BITMAPTEXTURE_H_

#include "platform/platformGL.h"

#include "base/types.h"
#include "texture.h"

class BitmapTexture : public Texture {
public:
	enum Format {
		FormatRGB8 = 3,
		FormatRGBA8 = 4
	};

	bool generated;
	GLuint buffer;
	glm::ivec2 extent;
	U8 *pixels;
	Format format;

	/**
	 Creates and allocates a Texture from a pixel array and extent
	 @arg pixels - A 4-component list of pixels in a RGBA list
	 @arg extent - A glm::ivec2 with the image extent
	 @arg format - The bitmap's format (Either RGB8 or RGBA8)
	 */
	BitmapTexture(U8 *pixels, const glm::ivec2 &extent, const Format &format);

	/**
	 Releases a Texture, freeing both its store and its buffer
	 */
	virtual ~BitmapTexture();

	/**
	 Generates the OpenGL buffer for a Texture. Don't call this before setting
	 up the OpenGL canvas!
	 */
	virtual void generateBuffer();

	/**
	 Activates a Texture for drawing with OpenGL and binds its buffer
	 */
	virtual void activate(GLenum texNum);

	/**
	 Deactivates a Texture after drawing
	 */
	virtual void deactivate();
};

#endif // _RENDER_TEXTURE_BITMAPTEXTURE_H_