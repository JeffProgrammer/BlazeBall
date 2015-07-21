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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/GLU.h>
#include <GL/glew.h>
#endif

#include "base/io.h"
#include "bitmap/texture.h"

#define TEXTURE_MAX_SIZE 1024

Texture::Texture(U8 *pixels, Point2I extent) {
	if (extent.x > TEXTURE_MAX_SIZE || extent.y > TEXTURE_MAX_SIZE) {
		printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", extent.x, extent.y, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE);
		return;
	}

	//Set some fields
	this->extent = extent;
	generated = false;

	//Load pixels into pixels (assume RGBA)
	this->pixels = new U8[extent.x * extent.y * 4];
	memcpy(this->pixels, pixels, sizeof(U8) * extent.x * extent.y * 4);
}

void Texture::generateBuffer() {
	//Just in case
	glEnable(GL_TEXTURE_2D);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, extent.x, extent.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	generated = true;
}

Texture::~Texture() {
	//Clean up
	if (generated)
		glDeleteTextures(1, &buffer);

	delete pixels;
}

void Texture::activate() {
	//Error check
	if (!generated)
		return;
	//Activate and bind the buffer
	glActiveTexture(texNum);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, buffer);
}

void Texture::deactivate() {
	//Haha, this method is just BS. Fooled you.
	glActiveTexture(0);
}

String Texture::find(String name, String directory) {
	//For some reason these two like to become the same.
	String base = directory;

	//Allocate enough space in each of these so we can work comfortably
	U32 pathlen = (U32)(base.length + name.length + 1);
	String imageFile = String(pathlen + 5);

	do {
		//Init imageFile to base/file.png
		pathlen = sprintf(imageFile, "%s/%s.png", (char *)base, (char *)name);

		//If we can't find the PNG, try for JPEG
		//TODO: BMP Support?
		if (!IO::isfile(imageFile)) {
			//Swap the last 3 chars with jpg
			imageFile.data[pathlen - 3] = 'j';
			imageFile.data[pathlen - 2] = 'p';
			imageFile.data[pathlen - 1] = 'g';
		}
		if (!IO::isfile(imageFile)) {
			//Swap the last 3 chars with jng
			imageFile.data[pathlen - 3] = 'j';
			imageFile.data[pathlen - 2] = 'n';
			imageFile.data[pathlen - 1] = 'g';
		}
		//Can't recurse any further
		if (!strrchr(base, '/'))
			break;

		//If we still can't find it, recurse (hacky but effective method)
		if (!IO::isfile(imageFile)) {
			*strrchr((const char *)base.data, '/') = 0;
		}
	} while (!IO::isfile(imageFile) && strcmp(base, ""));

	//If we can't find it, just chuck the lot and keep going.
	if (!IO::isfile(imageFile)) {
		return "";
	}
	return imageFile;
}