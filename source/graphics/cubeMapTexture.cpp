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
#include "graphics/cubeMapTexture.h"
#include "graphics/util.h"

#define TEXTURE_MAX_SIZE 1024

CubeMapTexture::CubeMapTexture(const std::vector<TextureInfo> &textureInfos) {
	for (const auto &texture : textureInfos) {
		if (texture.extent.x > TEXTURE_MAX_SIZE || texture.extent.y > TEXTURE_MAX_SIZE) {
			printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", texture.extent.x, texture.extent.y, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE);
			return;
		}

		mTextures.push_back(TextureInfo(texture)); //Copy the info into a new texture
	}
}

void CubeMapTexture::generateBuffer() {
	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &mBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mBuffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (const auto &texture : mTextures) {
		//Actually create the texture
		GLenum glformat = (texture.format == BitmapFormatRGB8 ? GL_RGB : GL_RGBA);
		glTexImage2D(texture.face, 0, glformat, texture.extent.x, texture.extent.y, 0, glformat, GL_UNSIGNED_BYTE, texture.pixels);
		delete [] texture.pixels;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// check for errors
	GL_CHECKERRORS();

	mGenerated = true;
}

CubeMapTexture::~CubeMapTexture() {
	//Clean up
	if (mGenerated)
		glDeleteTextures(1, &mTexNum);
}

void CubeMapTexture::activate() {
	//Error check
	if (!mGenerated)
		return;
	//Activate and bind the buffer
	glActiveTexture(mTexNum);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mBuffer);
}

void CubeMapTexture::deactivate() {
	//Haha, this method is just BS. Fooled you.
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
