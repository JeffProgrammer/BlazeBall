//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
#include "render/util.h"

BitmapTexture::BitmapTexture(U8 *pixels, const glm::ivec2 &extent, const Format &format) {
	if (extent.x > MaxTextureSize || extent.y > MaxTextureSize) {
		printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", extent.x, extent.y, MaxTextureSize, MaxTextureSize);
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

void BitmapTexture::activate(GLenum texNum) {
	//Error check
	if (!generated)
		generateBuffer();
	
	//Activate and bind the buffer
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_2D, buffer);
}

void BitmapTexture::deactivate() {
	//Haha, this method is just BS. Fooled you.
}
