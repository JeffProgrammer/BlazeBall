//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include "base/io.h"
#include "texture/cubeMapTexture.h"
#include "render/util.h"

CubeMapTexture::CubeMapTexture(const std::vector<TextureInfo> &textureInfos) {
	//Load each of the new textures into the cubemap
	for (const auto &texture : textureInfos) {
		if (texture.extent.x > MaxTextureSize || texture.extent.y > MaxTextureSize) {
			IO::printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.\n", texture.extent.x, texture.extent.y, MaxTextureSize, MaxTextureSize);
			return;
		}

		mTextures.push_back(TextureInfo(texture)); //Copy the info into a new texture
	}
	mGenerated = false;
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

	for (auto &texture : mTextures) {
		//Actually create the texture
		GLenum glformat = (texture.format == BitmapTexture::FormatRGB8 ? GL_RGB : GL_RGBA);
		glTexImage2D(texture.face, 0, glformat, texture.extent.x, texture.extent.y, 0, glformat, GL_UNSIGNED_BYTE, texture.pixels);
		delete [] texture.pixels;
		texture.pixels = nullptr;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// check for errors
	GL_CHECKERRORS();

	mGenerated = true;
}

CubeMapTexture::~CubeMapTexture() {
	//Clean up
	if (mGenerated) {
		glDeleteTextures(1, &mBuffer);
	}
}

void CubeMapTexture::activate(GLenum texNum) {
	//Error check
	if (!mGenerated)
		generateBuffer();

	//Activate and bind the buffer
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mBuffer);
}

void CubeMapTexture::deactivate() {
	//Haha, this method is just BS. Fooled you.
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
