//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/GL21/gl21.h"
#include "graphics/GL21/shaderTranslatorGL21.h"

GL21::GL21() {
	mTranslator = std::make_unique<ShaderTranslatorGL21>();
}

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
	epoxy_glDeleteVertexArraysAPPLE(count, array);
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
