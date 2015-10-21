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

#include <stdio.h>
#include <string>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

#include "base/io.h"
#include "bitmapTexture.h"
#include "graphics/util.h"

#define TEXTURE_MAX_SIZE 2048

BitmapTexture::BitmapTexture(U8 *pixels, const glm::ivec2 &extent, const Format &format) {
	if (extent.x > TEXTURE_MAX_SIZE || extent.y > TEXTURE_MAX_SIZE) {
		printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", extent.x, extent.y, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE);
		return;
	}

	//Set some fields
	this->extent = extent;
	this->format = format;
	generated = false;

	//Load pixels into pixels (assume RGBA)
	this->pixels = new U8[extent.x * extent.y * format];
	memcpy(this->pixels, pixels, sizeof(U8) * extent.x * extent.y * format);
}

void BitmapTexture::generateBuffer() {
	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &buffer);
	glBindTexture(GL_TEXTURE_2D, buffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Actually create the texture
	GLenum glformat = (format == FormatRGB8 ? GL_RGB : GL_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, glformat, extent.x, extent.y, 0, glformat, GL_UNSIGNED_BYTE, pixels);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	// check for errors
	GL_CHECKERRORS();

	delete [] pixels;
	generated = true;
}

BitmapTexture::~BitmapTexture() {
	//Clean up
	if (generated)
		glDeleteTextures(1, &buffer);
	else
		delete [] pixels;
}

void BitmapTexture::activate() {
	//Error check
	if (!generated)
		generateBuffer();
	
	//Activate and bind the buffer
	glActiveTexture(mTexNum);
	glBindTexture(GL_TEXTURE_2D, buffer);
}

void BitmapTexture::deactivate() {
	//Haha, this method is just BS. Fooled you.
}
