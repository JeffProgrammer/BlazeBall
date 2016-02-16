//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_TEXTURE_CUBEMAPTEXTURE_H_
#define _RENDER_TEXTURE_CUBEMAPTEXTURE_H_

#include <stdbool.h>
#include "platform/platformGL.h"
#include "base/types.h"
#include "bitmapTexture.h"
#include "base/io.h"

class CubeMapTexture : public Texture {
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
		BitmapTexture::Format format;
		CubeFace face;

		/**
		 * Load a TextureInfo from a texture for a given face. The texture will not be modified.
		 * @param texture The texture to use
		 * @param face The cube face for this texture on the cubemap
		 */
		TextureInfo(BitmapTexture const *texture, CubeFace face) {
			this->extent = texture->extent;
			this->pixels = new U8[texture->extent.x * texture->extent.y * texture->format];
			this->format = texture->format;
			this->face = face;
			memcpy(this->pixels, texture->pixels, sizeof(U8) * texture->extent.x * texture->extent.y * texture->format);
		}
		
		/**
		 * Copy a TextureInfo from another.
		 * @param info The TextureInfo to copy
		 */
		explicit TextureInfo(TextureInfo const *info) {
			this->extent = info->extent;
			this->pixels = new U8[info->extent.x * info->extent.y * info->format];
			this->format = info->format;
			this->face = info->face;
			memcpy(this->pixels, info->pixels, sizeof(U8) * info->extent.x * info->extent.y * info->format);
		}

		/**
		 * Copy a TextureInfo from another.
		 * @param info The TextureInfo to copy
		 */
		explicit TextureInfo(const TextureInfo &info) {
			this->extent = info.extent;
			this->pixels = new U8[info.extent.x * info.extent.y * info.format];
			this->format = info.format;
			this->face = info.face;
			memcpy(this->pixels, info.pixels, sizeof(U8) * info.extent.x * info.extent.y * info.format);
		}

		/**
		 * Load a TextureInfo from a given file for a given face.
		 * @param path The path of the file with the texture
		 * @param face The cube face for this texture on the cubemap
		 */
		TextureInfo(const std::string &path, CubeFace face) {
			BitmapTexture *texture = dynamic_cast<BitmapTexture *>(IO::loadTexture(path));
			this->extent = texture->extent;
			this->pixels = new U8[texture->extent.x * texture->extent.y * texture->format];
			this->format = texture->format;
			this->face = face;
			memcpy(this->pixels, texture->pixels, sizeof(U8) * texture->extent.x * texture->extent.y * texture->format);

			delete texture;
		}

		~TextureInfo() {
			if (pixels)
				delete [] pixels;
		}
	};

	GLuint mBuffer;
	std::vector<TextureInfo> mTextures;
	bool mGenerated;

	/**
	 * Creates and allocates a cubemap texture from a list of textures
	 * @param textureInfos A vector of TextureInfo objects containing texture data
	 */
	CubeMapTexture(const std::vector<TextureInfo> &textureInfos);

	/**
	 * Releases a cubemap texture, freeing both its store and its buffer
	 */
	virtual ~CubeMapTexture();

	/**
	 * Generates the OpenGL buffer for a Texture. Don't call this before setting
	 * up the OpenGL canvas!
	 */
	virtual void generateBuffer();

	/**
	 * Activates a Texture for drawing with OpenGL and binds its buffer
	 */
	virtual void activate(GLenum texNum);

	/**
	 * Deactivates a Texture after drawing
	 */
	virtual void deactivate();
};

#endif // _RENDER_TEXTURE_CUBEMAPTEXTURE_H_