//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_GL33_GL33_H_
#define _GRAPHICS_GL33_GL33_H_

#include "graphics/glImplementation.h"

class GL33 : public GL {
public:
	virtual void bindVertexArray(GLuint array) override;
	virtual void deleteVertexArrays(GLsizei count, const GLuint *array) override;
	virtual void genVertexArrays(GLsizei count, GLuint *arrays) override;
};

#endif // _GRAPHICS_GL33_GL33_H_