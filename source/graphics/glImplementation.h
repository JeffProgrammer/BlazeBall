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

#ifndef _GRAPHICS_GLIMPLEMENTATION_H_
#define _GRAPHICS_GLIMPLEMENTATION_H_

#include <memory>
#include <string>

#include <epoxy/gl.h>
#if defined(_WIN32)
#include <epoxy/wgl.h>
#elif defined(__linux__) || defined(__FREEBSD__)
#include <epoxy/glx.h>
#endif

#include "graphics/shaderTranslator.h"

class GL {
public:
	GL();

	/* OpenGL API functions*/
public:
	void activeTexture(GLenum texture);
	void attachShader(GLuint program, GLuint shader);

	void bindBuffer(GLenum target, GLuint buffer);
	void bindFrameBuffer(GLenum target, GLuint frameBuffer);
	void bindRenderBuffer(GLenum target, GLuint renderBuffer);
	void bindTexture(GLenum target, GLuint texture);
	virtual void bindVertexArray(GLuint array) = 0;
	void blendFunc(GLenum sfactor, GLenum dfactor);
	void bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

	void clear(GLbitfield bitField);
	void clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	GLenum checkFrameBufferStatus(GLenum target);
	void compileShader(GLuint shader);
	GLuint createShader(GLenum shaderType);
	GLuint createProgram();

	void deleteBuffers(GLsizei count, const GLuint *buffers);
	void deleteFramebuffers(GLsizei count, const GLuint *frameBuffers);
	void deleteProgram(GLuint program);
	void deleteRenderbuffers(GLsizei count, GLuint *renderbuffers);
	void deleteShader(GLuint shader);
	void deleteTextures(GLsizei count, const GLuint *textures);
	virtual void deleteVertexArrays(GLsizei count, const GLuint *array) = 0;
	void depthFunc(GLenum func);
	void depthMask(GLboolean flag);
	void disable(GLenum cap);
	void disableVertexAttribArray(GLuint index);
	void drawArrays(GLenum mode, GLint first, GLsizei count);
	virtual void drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primCount) = 0;
	void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
	
	void enable(GLenum cap);
	void enableVertexAttribArray(GLuint index);

	void framebufferRenderBuffer(GLenum target, GLenum attachment, GLenum renderbufferTarget, GLuint renderBuffer);
	void framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	void frontFace(GLenum mode);

	void genBuffers(GLsizei count, GLuint *buffers);
	void generateMipMap(GLenum target);
	void genFrameBuffers(GLsizei count, GLuint *frameBuffers);
	void genRenderBuffers(GLsizei count, GLuint *renderBuffers);
	void genTextures(GLsizei count, GLuint *textures);
	virtual void genVertexArrays(GLsizei count, GLuint *arrays) = 0;
	GLint getAttribLocation(GLuint program, const GLchar *name);
	GLenum getError();
	void getIntegerv(GLenum pname, GLint *data);
	void getProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
	void getProgramiv(GLuint program, GLenum pname, GLint *params);
	void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
	void getShaderiv(GLuint shader, GLenum pname, GLint *params);
	const GLubyte *getString(GLenum name);
	GLint getUniformLocation(GLuint program, const GLchar *name);
	
	void linkProgram(GLuint program);

	void multiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawCount);

	void pixelStorei(GLenum pname, GLint param);

	void renderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height);

	void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
	void shaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);

	void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
	void texParameteri(GLenum target, GLenum pname, GLint param);

	void uniform1f(GLint location, GLfloat v0);
	void uniform1i(GLint location, GLint v0);
	void uniform2fv(GLint location, GLsizei count, const GLfloat *value);
	void uniform3fv(GLint location, GLsizei count, const GLfloat *value);
	void uniform4fv(GLint location, GLsizei count, const GLfloat *value);
	void uniform2iv(GLint location, GLsizei count, const GLint *value);
	void uniform3iv(GLint location, GLsizei count, const GLint *value);
	void uniform4iv(GLint location, GLsizei count, const GLint *value);
	void uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void useProgram(GLuint program);

	void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	virtual void vertexAttribDivisor(GLuint index, GLuint divisor) = 0;
	void viewport(GLint x, GLint y, GLsizei width, GLsizei height);

/* Custom extensions*/
public:
	void getViewportEXT(GLint &x, GLint &y, GLsizei &width, GLsizei &height);
	void bindContextEXT(void *context);
	GLuint getVideoRamEXT();
	void setPixelDensityEXT(GLfloat pixelDensity);
	GLfloat getPixelDensityEXT();
	std::string translateShaderEXT(const std::string &source, GLenum type);

/**
 * Public Static functions available from the GL "namespace"
 */
public:
	static std::unique_ptr<GL> sGL;

	template<typename API>
	static void createGL() {
		sGL = std::make_unique<API>();
	}

protected:
	struct Viewport {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;

		Viewport(const GLint &x, const GLint &y, const GLsizei &width, const GLsizei &height) : x(x), y(y), width(width), height(height) {}
		Viewport() {}
	} mViewport;
	
	void *mCurrentContext;

	GLfloat mPixelDensity;

	std::unique_ptr<ShaderTranslator> mTranslator;
};

#endif // _GRAPHICS_GLIMPLEMENTATION_H_