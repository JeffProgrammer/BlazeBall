//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/GL21/gl21.h"
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

void GL21::bindVertexArray(GLuint array) {
#ifdef __APPLE__
	glBindVertexArrayAPPLE(array);
#else
	// Requires OpenGL 3.0 or ARB_VERTEX_ARRAY_OBJECT
	glBindVertexArray(array);
#endif
}

void GL21::deleteVertexArrays(GLsizei count, const GLuint *array) {
#ifdef __APPLE__
	glDeleteVertexArraysAPPLE(count, arrays);
#else
	// Requires OpenGL 3.0 or ARB_VERTEX_ARRAY_OBJECT
	glDeleteVertexArrays(count, array);
#endif
}

void GL21::genVertexArrays(GLsizei count, GLuint *arrays) {
#ifdef __APPLE__
	glGenVertexArraysAPPLE(count, arrays);
#else
	// Requires OpenGL 3.0 or ARB_VERTEX_ARRAY_OBJECT
	glGenVertexArrays(count, arrays);
#endif
}
