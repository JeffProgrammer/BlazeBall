//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef bitmapTexture_h
#define bitmapTexture_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif
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

#endif
