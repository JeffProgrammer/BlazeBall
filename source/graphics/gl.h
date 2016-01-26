//-----------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeffrey Hutchinson
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _GRAPHICS_GL_GL_H_
#define _GRAPHICS_GL_GL_H_

#include "graphics/glImplementation.h"

//-----------------------------------------------------------------------------
// OpenGL Functions and extensions
//-----------------------------------------------------------------------------

inline void glActiveTexture(GLenum texture) {
	GL::sGL->activeTexture(texture);
}

inline void glAttachShader(GLuint program, GLuint shader) {
	GL::sGL->attachShader(program, shader);
}

inline void glBindBuffer(GLenum target, GLuint buffer) {
	GL::sGL->bindBuffer(target, buffer);
}

inline void glBindFramebuffer(GLenum target, GLuint frameBuffer) {
	GL::sGL->bindFrameBuffer(target, frameBuffer);
}

inline void glBindRenderbuffer(GLenum target, GLuint renderBuffer) {
	GL::sGL->bindRenderBuffer(target, renderBuffer);
}

inline void glBindTexture(GLenum target, GLuint texture) {
	GL::sGL->bindTexture(target, texture);
}

inline void glBindVertexArray(GLuint array) {
	GL::sGL->bindVertexArray(array);
}

inline void glBlendFunc(GLenum sfactor, GLenum dfactor) {
	GL::sGL->blendFunc(sfactor, dfactor);
}

inline void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
	GL::sGL->bufferData(target, size, data, usage);
}

inline void glClear(GLbitfield bitField) {
	GL::sGL->clear(bitField);
}

inline void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	GL::sGL->clearColor(red, green, blue, alpha);
}

inline GLenum glCheckFramebufferStatus(GLenum target) {
	return GL::sGL->checkFrameBufferStatus(target);
}

inline void glCompileShader(GLuint shader) {
	GL::sGL->compileShader(shader);
}

inline GLuint glCreateShader(GLenum shaderType) {
	return GL::sGL->createShader(shaderType);
}

inline GLuint glCreateProgram() {
	return GL::sGL->createProgram();
}

inline void glDeleteBuffers(GLsizei count, const GLuint *buffers) {
	GL::sGL->deleteBuffers(count, buffers);
}

inline void glDeleteFramebuffers(GLsizei count, const GLuint *frameBuffers) {
	GL::sGL->deleteFramebuffers(count, frameBuffers);
}

inline void glDeleteProgram(GLuint program) {
	GL::sGL->deleteProgram(program);
}

inline void glDeleteRenderbuffers(GLsizei count, GLuint *renderbuffers) {
	GL::sGL->deleteRenderbuffers(count, renderbuffers);
}

inline void glDeleteShader(GLuint shader) {
	GL::sGL->deleteShader(shader);
}

inline void glDeleteTextures(GLsizei count, const GLuint *textures) {
	GL::sGL->deleteTextures(count, textures);
}

inline void glDeleteVertexArrays(GLsizei count, const GLuint *array) {
	GL::sGL->deleteVertexArrays(count, array);
}

inline void glDepthFunc(GLenum func) {
	GL::sGL->depthFunc(func);
}

inline void glDisable(GLenum cap) {
	GL::sGL->disable(cap);
}

inline void glDisableVertexAttribArray(GLuint index) {
	GL::sGL->disableVertexAttribArray(index);
}

inline void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
	GL::sGL->drawArrays(mode, first, count);
}

inline void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
	GL::sGL->drawElements(mode, count, type, indices);
} 

inline void glEnable(GLenum cap) {
	GL::sGL->enable(cap);
}

inline void glEnableVertexAttribArray(GLuint index) {
	GL::sGL->enableVertexAttribArray(index);
}

inline void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbufferTarget, GLuint renderBuffer) {
	GL::sGL->framebufferRenderBuffer(target, attachment, renderbufferTarget, renderBuffer);
}

inline void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
	GL::sGL->framebufferTexture2D(target, attachment, textarget, texture, level);
}

inline void glGenBuffers(GLsizei count, GLuint *buffers) {
	GL::sGL->genBuffers(count, buffers);
}

inline void glGenerateMipmap(GLenum target) {
	GL::sGL->generateMipMap(target);
}

inline void glGenFramebuffers(GLsizei count, GLuint *frameBuffers) {
	GL::sGL->genFrameBuffers(count, frameBuffers);
}

inline void glGenRenderbuffers(GLsizei count, GLuint *renderBuffers) {
	GL::sGL->genRenderBuffers(count, renderBuffers);
}

inline void glGenTextures(GLsizei count, GLuint *textures) {
	GL::sGL->genTextures(count, textures);
}

inline void glGenVertexArrays(GLsizei count, GLuint *arrays) {
	GL::sGL->genVertexArrays(count, arrays);
}

inline GLint glGetAttribLocation(GLuint program, const GLchar *name) {
	return GL::sGL->getAttribLocation(program, name);
}

inline GLenum glGetError() {
	return GL::sGL->getError();
}

inline void glGetIntegerv(GLenum pname, GLint *data) {
	GL::sGL->getIntegerv(pname, data);
}

inline void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
	GL::sGL->getProgramInfoLog(program, maxLength, length, infoLog);
}

inline void glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
	GL::sGL->getProgramiv(program, pname, params);
}

inline void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
	GL::sGL->getShaderInfoLog(shader, maxLength, length, infoLog);
}

inline void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
	GL::sGL->getShaderiv(shader, pname, params);
}

inline const GLubyte* glGetString(GLenum name) {
	return GL::sGL->getString(name);
}

inline GLint glGetUniformLocation(GLuint program, const GLchar *name) {
	return GL::sGL->getUniformLocation(program, name);
}

inline void glLinkProgram(GLuint program) {
	GL::sGL->linkProgram(program);
}

inline void glPixelStorei(GLenum pname, GLint param) {
	GL::sGL->pixelStorei(pname, param);
}

inline void glRenderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height) {
	GL::sGL->renderbufferStorage(target, internalFormat, width, height);
}

inline void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
	GL::sGL->scissor(x, y, width, height);
}

inline void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
	GL::sGL->shaderSource(shader, count, string, length);
}

inline void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data) {
	GL::sGL->texImage2D(target, level, internalFormat, width, height, border, format, type, data);
}

inline void glTexParameteri(GLenum target, GLenum pname, GLint param) {
	GL::sGL->texParameteri(target, pname, param);
}

inline void glUniform1f(GLint location, GLfloat v0) {
	GL::sGL->uniform1f(location, v0);
}

inline void glUniform1i(GLint location, GLint v0) {
	GL::sGL->uniform1i(location, v0);
}

inline void glUniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	GL::sGL->uniform2fv(location, count, value);
}

inline void glUniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	GL::sGL->uniform3fv(location, count, value);
}

inline void glUniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	GL::sGL->uniform4fv(location, count, value);
}

inline void glUniform2iv(GLint location, GLsizei count, const GLint *value) {
	GL::sGL->uniform2iv(location, count, value);
}

inline void glUniform3iv(GLint location, GLsizei count, const GLint *value) {
	GL::sGL->uniform3iv(location, count, value);
}

inline void glUniform4iv(GLint location, GLsizei count, const GLint *value) {
	GL::sGL->uniform4iv(location, count, value);
}

inline void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	GL::sGL->uniformMatrix3fv(location, count, transpose, value);
}

inline void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	GL::sGL->uniformMatrix4fv(location, count, transpose, value);
}

inline void glUseProgram(GLuint program) {
	GL::sGL->useProgram(program);
}

inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	GL::sGL->vertexAttribPointer(index, size, type, normalized, stride, pointer);
}

inline void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	GL::sGL->viewport(x, y, width, height);
}

//-----------------------------------------------------------------------------
// Custom OpenGL Extensions written for this multiple version OpenGL 
// implementation.
//-----------------------------------------------------------------------------

/**
 * Get's the current viewport without having any OpenGL driver overhead.
 * @param x OUT The x position of the viewport.
 * @param y OUT The y position of the viewport.
 * @param width OUT The width of the viewport.
 * @param height OUT The height of the viewport.
 */
inline void glGetViewportEXT(GLint &x, GLint &y, GLsizei &width, GLsizei &height) {
	GL::sGL->getViewportEXT(x, y, width, height);
}

#endif // _GRAPHICS_GL_GL_H_