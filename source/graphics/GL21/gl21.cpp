//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

void GL21::drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primCount) {
	// Requires OpenGL 3.1 or Instance support
	epoxy_glDrawArraysInstancedARB(mode, first, count, primCount);
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

void GL21::vertexAttribDivisor(GLuint index, GLuint divisor) {
	// Requires OpenGL 3.3 or ARB_INSTANCED_ARRAYS
	epoxy_glVertexAttribDivisorARB(index, divisor);
}
