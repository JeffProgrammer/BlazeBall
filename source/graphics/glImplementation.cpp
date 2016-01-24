//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/gl.h"

GL::GL() {
	mViewport = Viewport(0, 0, 0, 0);
}

void GL::bindBuffer(GLenum target, GLuint buffer) {
	glBindBuffer(target, buffer);
}

void GL::bindFrameBuffer(GLenum target, GLuint frameBuffer) {
	glBindFramebuffer(target, frameBuffer);
}

void GL::bindRenderBuffer(GLenum target, GLuint renderBuffer) {
	glBindRenderbuffer(target, renderBuffer);
}

void GL::bindTexture(GLenum target, GLuint texture) {
	glBindTexture(target, texture);
}

void GL::clear(GLbitfield bitField) {
	glClear(bitField);
}

void GL::clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClearColor(red, green, blue, alpha);
}

void GL::compileShader(GLuint shader) {
	glCompileShader(shader);
}

GLuint GL::createShader(GLenum shaderType) {
	return glCreateShader(shaderType);
}

GLuint GL::createProgram() {
	return glCreateProgram();
}

void GL::deleteBuffers(GLsizei count, const GLuint *buffers) {
	glDeleteBuffers(count, buffers);
}

void GL::deleteFramebuffers(GLsizei count, const GLuint *frameBuffers) {
	glDeleteFramebuffers(count, frameBuffers);
}

void GL::deleteProgram(GLuint program) {
	glDeleteProgram(program);
}

void GL::deleteRenderbuffers(GLsizei count, GLuint *renderbuffers) {
	glDeleteRenderbuffers(count, renderbuffers);
}

void GL::deleteShader(GLuint shader) {
	glDeleteShader(shader);
}

void GL::deleteTextures(GLsizei count, const GLuint *textures) {
	glDeleteTextures(count, textures);
}

void GL::depthFunc(GLenum func) {
	glDepthFunc(func);
}

void GL::disable(GLenum cap) {
	glDisable(cap);
}

void GL::disableVertexAttribArray(GLuint index) {
	glDisableVertexAttribArray(index);
}

void GL::drawArrays(GLenum mode, GLint first, GLsizei count) {
	glDrawArrays(mode, first, count);
}

void GL::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
	glDrawElements(mode, count, type, indices);
}

void GL::enable(GLenum cap) {
	glEnable(cap);
}

void GL::enableVertexAttribArray(GLuint index) {
	glEnableVertexAttribArray(index);
}

void GL::framebufferRenderBuffer(GLenum target, GLenum attachment, GLenum renderbufferTarget, GLuint renderBuffer) {
	glFramebufferRenderbuffer(target, attachment, renderbufferTarget, renderBuffer);
}

void GL::framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void GL::genBuffers(GLsizei count, GLuint *buffers) {
	glGenBuffers(count, buffers);
}

void GL::generateMipMap(GLenum target) {
	glGenerateMipmap(target);
}

void GL::genFrameBuffers(GLsizei count, GLuint *frameBuffers) {
	glGenFramebuffers(count, frameBuffers);
}

void GL::genRenderBuffers(GLsizei count, GLuint *renderBuffers) {
	glGenRenderbuffers(count, renderBuffers);
}

void GL::genTextures(GLsizei count, GLuint *textures) {
	glGenTextures(count, textures);
}

GLenum GL::getError() {
	return glGetError();
}

void GL::getIntegerv(GLenum pname, GLint *data) {
	glGetIntegerv(pname, data);
}

void GL::getProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
	glGetProgramInfoLog(program, maxLength, length, infoLog);
}

void GL::getProgramiv(GLuint program, GLenum pname, GLint *params) {
	glGetProgramiv(program, pname, params);
}

void GL::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
	glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

void GL::getShaderiv(GLuint shader, GLenum pname, GLint *params) {
	glGetShaderiv(shader, pname, params);
}

void GL::getUniformLocation(GLuint program, const GLchar *name) {
	glGetUniformLocation(program, name);
}

void GL::linkProgram(GLuint program) {
	glLinkProgram(program);
}

void GL::pixelStorei(GLenum pname, GLint param) {
	glPixelStorei(pname, param);
}

void GL::renderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height) {
	glRenderbufferStorage(target, internalFormat, width, height);
}

void GL::shaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
	glShaderSource(shader, count, string, length);
}

void GL::texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data) {
	glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
}

void GL::texParameteri(GLenum target, GLenum pname, GLint param) {
	glTexParameteri(target, pname, param);
}

void GL::uniform1f(GLint location, GLfloat v0) {
	glUniform1f(location, v0);
}

void GL::uniform1i(GLint location, GLint v0) {
	glUniform1i(location, v0);
}

void GL::uniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform2fv(location, count, value);
}

void GL::uniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform3fv(location, count, value);
}

void GL::uniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform4fv(location, count, value);
}

void GL::uniform2iv(GLint location, GLsizei count, const GLint *value) {
	glUniform2iv(location, count, value);
}

void GL::uniform3iv(GLint location, GLsizei count, const GLint *value) {
	glUniform3iv(location, count, value);
}

void GL::uniform4iv(GLint location, GLsizei count, const GLint *value) {
	glUniform4iv(location, count, value);
}

void GL::uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3fv(location, count, transpose, value);
}

void GL::uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4fv(location, count, transpose, value);
}

void GL::useProgram(GLuint program) {
	glUseProgram(program);
}

void GL::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void GL::viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	mViewport = Viewport(x, y, width, height);
	glViewport(x, y, width, height);
}

// Custom Extensions

void GL::getViewportEXT(GLint &x, GLint &y, GLsizei &width, GLsizei &height) {
	x = mViewport.x;
	y = mViewport.y;
	width = mViewport.width;
	height = mViewport.height;
}