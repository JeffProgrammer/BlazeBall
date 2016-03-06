//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_GL21_GL21_H_
#define _GRAPHICS_GL21_GL21_H_

#include "graphics/glImplementation.h"

class GL21 : public GL {
public:
	GL21();

	virtual void bindVertexArray(GLuint array) override;
	virtual void drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primCount) override;
	virtual void deleteVertexArrays(GLsizei count, const GLuint *array) override;
	virtual void genVertexArrays(GLsizei count, GLuint *arrays) override;
	virtual void vertexAttribDivisor(GLuint index, GLuint divisor) override;
}; 

#endif // _GRAPHICS_GL21_GL21_H_