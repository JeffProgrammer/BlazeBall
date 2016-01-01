//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef texture_h
#define texture_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
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


#endif /* texture_h */
