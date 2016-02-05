//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_TEXTURE_TEXTURE_H_
#define _RENDER_TEXTURE_TEXTURE_H_

#include "platform/platformGL.h"
#include "base/types.h"

class Texture {
public:
	static const U32 MaxTextureSize = 2048;

	Texture() {};

	/**
	 Releases a Texture, freeing both its store and its buffer
	 */
	virtual ~Texture() {};

	/**
	 Generates the OpenGL buffer for a Texture. Don't call this before setting
	 up the OpenGL canvas!
	 */
	virtual void generateBuffer() = 0;

	/**
	 Activates a Texture for drawing with OpenGL and binds its buffer
	 */
	virtual void activate(GLenum texNum) = 0;

	/**
	 Deactivates a Texture after drawing
	 */
	virtual void deactivate() = 0;

	/**
	 Find a texture in a directory
	 */
	static std::string find(const std::string &fullName);
};

#endif // _RENDER_TEXTURE_TEXTURE_H_