//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/GL33/gl33.h"

void GL33::bindVertexArray(GLuint array) {
	epoxy_glBindVertexArray(array);
}

void GL33::deleteVertexArrays(GLsizei count, const GLuint *array) {
	epoxy_glDeleteVertexArrays(count, array);
}

void GL33::genVertexArrays(GLsizei count, GLuint *arrays) {
	epoxy_glGenVertexArrays(count, arrays);
}
