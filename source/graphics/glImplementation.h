//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_GL_GLIMPLEMENTATION_H_
#define _GRAPHICS_GL_GLIMPLEMENTATION_H_

#include <memory>
#include <string>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <Windows.h>
#include <GL/GL.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#endif

class GL {
public:
	GL();

	/* OpenGL API functions*/
public:
	void bindBuffer(GLenum target, GLuint buffer);
	void bindFrameBuffer(GLenum target, GLuint frameBuffer);
	void bindRenderBuffer(GLenum target, GLuint renderBuffer);
	void bindTexture(GLenum target, GLuint texture);
	virtual void bindVertexArray(GLuint array) = 0;

	void clear(GLbitfield bitField);
	void clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
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
	void disable(GLenum cap);
	void disableVertexAttribArray(GLuint index);
	void drawArrays(GLenum mode, GLint first, GLsizei count);
	void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices); 
	
	void enable(GLenum cap);
	void enableVertexAttribArray(GLuint index);

	void framebufferRenderBuffer(GLenum target, GLenum attachment, GLenum renderbufferTarget, GLuint renderBuffer);
	void framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

	void genBuffers(GLsizei count, GLuint *buffers);
	void generateMipMap(GLenum target);
	void genFrameBuffers(GLsizei count, GLuint *frameBuffers);
	void genRenderBuffers(GLsizei count, GLuint *renderBuffers);
	void genTextures(GLsizei count, GLuint *textures);
	virtual void genVertexArrays(GLsizei count, GLuint *arrays) = 0;
	GLenum getError();
	void getIntegerv(GLenum pname, GLint *data);
	void getProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
	void getProgramiv(GLuint program, GLenum pname, GLint *params);
	void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
	void getShaderiv(GLuint shader, GLenum pname, GLint *params);
	void getUniformLocation(GLuint program, const GLchar *name);
	
	void linkProgram(GLuint program);

	void pixelStorei(GLenum pname, GLint param);

	void renderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height);

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
	void viewport(GLint x, GLint y, GLsizei width, GLsizei height);

/* Custom extensions*/
public:
	void getViewportEXT(GLint &x, GLint &y, GLsizei &width, GLsizei &height);

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
};

#endif // _GRAPHICS_GL_GLIMPLEMENTATION_H_