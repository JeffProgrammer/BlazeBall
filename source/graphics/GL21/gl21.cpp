//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/GL21/gl21.h"

void GL21::bindVertexArray(GLuint array) {
#ifdef __APPLE__
	epoxy_glBindVertexArrayAPPLE(array);
#else
	// Requires OpenGL 3.0 or ARB_VERTEX_ARRAY_OBJECT
	epoxy_glBindVertexArray(array);
#endif
}

void GL21::deleteVertexArrays(GLsizei count, const GLuint *array) {
#ifdef __APPLE__
	epoxy_glDeleteVertexArraysAPPLE(count, arrays);
#else
	// Requires OpenGL 3.0 or ARB_VERTEX_ARRAY_OBJECT
	epoxy_glDeleteVertexArrays(count, array);
#endif
}

void GL21::genVertexArrays(GLsizei count, GLuint *arrays) {
#ifdef __APPLE__
	epoxy_glGenVertexArraysAPPLE(count, arrays);
#else
	// Requires OpenGL 3.0 or ARB_VERTEX_ARRAY_OBJECT
	epoxy_glGenVertexArrays(count, arrays);
#endif
}
