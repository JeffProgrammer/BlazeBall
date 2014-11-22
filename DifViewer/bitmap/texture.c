//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
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
#include <OpenGL/glu.h>
#include "texture.h"

#define TEXTURE_MAX_SIZE 1024

Texture *texture_create_from_pixels(U8 *pixels, Point2I extent) {
	if (extent.x > TEXTURE_MAX_SIZE || extent.y > TEXTURE_MAX_SIZE) {
		printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", extent.x, extent.y, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE);
		return NULL;
	}

	Texture *texture = malloc(sizeof(Texture));

	//Set some fields
	texture->extent = extent;
	texture->generated = false;

	//Load pixels into texture->pixels (assume RGBA)
	texture->pixels = malloc(sizeof(U8) * extent.x * extent.y * 4);
	memcpy(texture->pixels, pixels, sizeof(U8) * extent.x * extent.y * 4);

	return texture;
}

void texture_generate_buffer(Texture *texture) {
	//Just in case
	glEnable(GL_TEXTURE_2D);

	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &texture->buffer);
	glBindTexture(GL_TEXTURE_2D, texture->buffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Actually create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->extent.x, texture->extent.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);

	texture->generated = true;
}

void texture_release(Texture *texture) {
	//Clean up
	if (texture->generated)
		glDeleteTextures(1, &texture->buffer);

	free(texture->pixels);
	free(texture);
}

void texture_activate(Texture *texture) {
	//Error check
	if (!texture->generated)
		return;
	//Activate and bind the buffer
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texture->buffer);
}

void texture_deactivate(Texture *texture) {
	//Haha, this method is just BS. Fooled you.
	glDisable(GL_TEXTURE_2D);
}
