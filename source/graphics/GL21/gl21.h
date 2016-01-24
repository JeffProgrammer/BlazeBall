//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_GL21_GL21_H_
#define _GRAPHICS_GL21_GL21_H_

#include "graphics/gl.h"

class GL21 : public GL {
public:
	virtual void bindVertexArray(GLuint array);
	virtual void deleteVertexArrays(GLsizei count, const GLuint *array);
	virtual void genVertexArrays(GLsizei count, GLuint *arrays);
};

#endif // _GRAPHICS_GL21_GL21_H_