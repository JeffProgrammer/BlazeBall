//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/GL33/gl33.h"
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

void GL33::bindVertexArray(GLuint array) {
	glBindVertexArray(array);
}

void GL33::deleteVertexArrays(GLsizei count, const GLuint *array) {
	glDeleteVertexArrays(count, array);
}

void GL33::genVertexArrays(GLsizei count, GLuint *arrays) {
	glGenVertexArrays(count, arrays);
}
