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

#ifndef cubeMapTexture_h
#define cubeMapTexture_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif
#include <stdbool.h>
#include "base/types.h"
#include "texture.h"

class CubeMapTexture {
public:
	enum CubeFace {
		PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	struct TextureInfo {
		U8 *pixels;
		glm::ivec2 extent;
		Texture::BitmapFormat format;
		CubeFace face;

		TextureInfo() {

		}
		TextureInfo(Texture *texture, CubeFace face) {
			this->extent = texture->extent;
			this->pixels = new U8[texture->extent.x * texture->extent.y * texture->format];
			this->format = texture->format;
			this->face = face;
			memcpy(this->pixels, texture->pixels, sizeof(U8) * texture->extent.x * texture->extent.y * texture->format);
		}
		TextureInfo(const TextureInfo &info) {
			this->extent = info.extent;
			this->pixels = new U8[info.extent.x * info.extent.y * info.format];
			this->format = info.format;
			this->face = info.face;
			memcpy(this->pixels, info.pixels, sizeof(U8) * info.extent.x * info.extent.y * info.format);
		}
	};

	GLenum mTexNum;
	GLuint mBuffer;
	std::vector<TextureInfo> mTextures;
	bool mGenerated;

	/**
	 Creates and allocates a Texture from a pixel array and extent
	 @arg pixels - A 4-component list of pixels in a RGBA list
	 @arg extent - A glm::ivec2 with the image extent
	 @arg format - The bitmap's format (Either RGB8 or RGBA8)
	 */
	CubeMapTexture(const std::vector<TextureInfo> &textureInfos);

	/**
	 Releases a Texture, freeing both its store and its buffer
	 */
	~CubeMapTexture();

	void setTexNum(const GLenum &texNum) {
		this->mTexNum = texNum;
	}

	GLenum getTexNum() {
		return mTexNum;
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
