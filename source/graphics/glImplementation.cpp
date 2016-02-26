//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "graphics/gl.h"
#include "platform/platform.h"

//#ifdef __APPLE__
#include "graphics/glMac.h"
//#endif

std::unique_ptr<GL> GL::sGL = nullptr;

GL::GL() {
	mViewport = Viewport(0, 0, 0, 0);
	mCurrentContext = nullptr;
	mPixelDensity = 1.0f;
}

void GL::activeTexture(GLenum texture) {
	epoxy_glActiveTexture(texture);
}

void GL::attachShader(GLuint program, GLuint shader) {
	epoxy_glAttachShader(program, shader);
}

void GL::bindBuffer(GLenum target, GLuint buffer) {
	epoxy_glBindBuffer(target, buffer);
}

void GL::bindFrameBuffer(GLenum target, GLuint frameBuffer) {
	epoxy_glBindFramebuffer(target, frameBuffer);
}

void GL::bindRenderBuffer(GLenum target, GLuint renderBuffer) {
	epoxy_glBindRenderbuffer(target, renderBuffer);
}

void GL::bindTexture(GLenum target, GLuint texture) {
	epoxy_glBindTexture(target, texture);
}

void GL::blendFunc(GLenum sfactor, GLenum dfactor) {
	epoxy_glBlendFunc(sfactor, dfactor);
}

void GL::bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
	epoxy_glBufferData(target, size, data, usage);
}

void GL::clear(GLbitfield bitField) {
	epoxy_glClear(bitField);
}

void GL::clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	epoxy_glClearColor(red, green, blue, alpha);
}

GLenum GL::checkFrameBufferStatus(GLenum target) {
	return epoxy_glCheckFramebufferStatus(target);
}

void GL::compileShader(GLuint shader) {
	epoxy_glCompileShader(shader);
}

GLuint GL::createShader(GLenum shaderType) {
	return epoxy_glCreateShader(shaderType);
}

GLuint GL::createProgram() {
	return epoxy_glCreateProgram();
}

void GL::deleteBuffers(GLsizei count, const GLuint *buffers) {
	epoxy_glDeleteBuffers(count, buffers);
}

void GL::deleteFramebuffers(GLsizei count, const GLuint *frameBuffers) {
	epoxy_glDeleteFramebuffers(count, frameBuffers);
}

void GL::deleteProgram(GLuint program) {
	epoxy_glDeleteProgram(program);
}

void GL::deleteRenderbuffers(GLsizei count, GLuint *renderbuffers) {
	epoxy_glDeleteRenderbuffers(count, renderbuffers);
}

void GL::deleteShader(GLuint shader) {
	epoxy_glDeleteShader(shader);
}

void GL::deleteTextures(GLsizei count, const GLuint *textures) {
	epoxy_glDeleteTextures(count, textures);
}

void GL::depthFunc(GLenum func) {
	epoxy_glDepthFunc(func);
}

void GL::disable(GLenum cap) {
	epoxy_glDisable(cap);
}

void GL::disableVertexAttribArray(GLuint index) {
	epoxy_glDisableVertexAttribArray(index);
}

void GL::drawArrays(GLenum mode, GLint first, GLsizei count) {
	epoxy_glDrawArrays(mode, first, count);
}

void GL::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
	epoxy_glDrawElements(mode, count, type, indices);
}

void GL::enable(GLenum cap) {
	epoxy_glEnable(cap);
}

void GL::enableVertexAttribArray(GLuint index) {
	epoxy_glEnableVertexAttribArray(index);
}

void GL::framebufferRenderBuffer(GLenum target, GLenum attachment, GLenum renderbufferTarget, GLuint renderBuffer) {
	epoxy_glFramebufferRenderbuffer(target, attachment, renderbufferTarget, renderBuffer);
}

void GL::framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
	epoxy_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void GL::genBuffers(GLsizei count, GLuint *buffers) {
	epoxy_glGenBuffers(count, buffers);
}

void GL::generateMipMap(GLenum target) {
	epoxy_glGenerateMipmap(target);
}

void GL::genFrameBuffers(GLsizei count, GLuint *frameBuffers) {
	epoxy_glGenFramebuffers(count, frameBuffers);
}

void GL::genRenderBuffers(GLsizei count, GLuint *renderBuffers) {
	epoxy_glGenRenderbuffers(count, renderBuffers);
}

void GL::genTextures(GLsizei count, GLuint *textures) {
	epoxy_glGenTextures(count, textures);
}

GLint GL::getAttribLocation(GLuint program, const GLchar *name) {
	return epoxy_glGetAttribLocation(program, name);
}

GLenum GL::getError() {
	return epoxy_glGetError();
}

void GL::getIntegerv(GLenum pname, GLint *data) {
	epoxy_glGetIntegerv(pname, data);
}

void GL::getProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
	epoxy_glGetProgramInfoLog(program, maxLength, length, infoLog);
}

void GL::getProgramiv(GLuint program, GLenum pname, GLint *params) {
	epoxy_glGetProgramiv(program, pname, params);
}

void GL::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
	epoxy_glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

void GL::getShaderiv(GLuint shader, GLenum pname, GLint *params) {
	epoxy_glGetShaderiv(shader, pname, params);
}

const GLubyte* GL::getString(GLenum name) {
	return epoxy_glGetString(name);
}

GLint GL::getUniformLocation(GLuint program, const GLchar *name) {
	return epoxy_glGetUniformLocation(program, name);
}

void GL::linkProgram(GLuint program) {
	epoxy_glLinkProgram(program);
}

void GL::pixelStorei(GLenum pname, GLint param) {
	epoxy_glPixelStorei(pname, param);
}

void GL::renderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height) {
	epoxy_glRenderbufferStorage(target, internalFormat, width, height);
}

void GL::scissor(GLint x, GLint y, GLsizei width, GLsizei height) {
	epoxy_glScissor(x, y, width, height);
}

void GL::shaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
	epoxy_glShaderSource(shader, count, string, length);
}

void GL::texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data) {
	epoxy_glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
}

void GL::texParameteri(GLenum target, GLenum pname, GLint param) {
	epoxy_glTexParameteri(target, pname, param);
}

void GL::uniform1f(GLint location, GLfloat v0) {
	epoxy_glUniform1f(location, v0);
}

void GL::uniform1i(GLint location, GLint v0) {
	epoxy_glUniform1i(location, v0);
}

void GL::uniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	epoxy_glUniform2fv(location, count, value);
}

void GL::uniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	epoxy_glUniform3fv(location, count, value);
}

void GL::uniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	epoxy_glUniform4fv(location, count, value);
}

void GL::uniform2iv(GLint location, GLsizei count, const GLint *value) {
	epoxy_glUniform2iv(location, count, value);
}

void GL::uniform3iv(GLint location, GLsizei count, const GLint *value) {
	epoxy_glUniform3iv(location, count, value);
}

void GL::uniform4iv(GLint location, GLsizei count, const GLint *value) {
	epoxy_glUniform4iv(location, count, value);
}

void GL::uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	epoxy_glUniformMatrix3fv(location, count, transpose, value);
}

void GL::uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	epoxy_glUniformMatrix4fv(location, count, transpose, value);
}

void GL::useProgram(GLuint program) {
	epoxy_glUseProgram(program);
}

void GL::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	epoxy_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void GL::viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	mViewport = Viewport(x, y, width, height);
	epoxy_glViewport(x, y, width, height);
}

// Custom Extensions

void GL::getViewportEXT(GLint &x, GLint &y, GLsizei &width, GLsizei &height) {
	x = mViewport.x;
	y = mViewport.y;
	width = mViewport.width;
	height = mViewport.height;
}

void GL::bindContextEXT(void *context) {
	mCurrentContext = context;
}

GLuint GL::getVideoRamEXT() {
#ifdef __APPLE__
	return (GLuint)getVideoMemoryAPPLE(mCurrentContext);
#else
	// NVidia cards
	if (epoxy_has_gl_extension("GL_NVX_gpu_memory_info")) {
		GLint memory;
		epoxy_glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &memory);

		// check for zero
		if (memory == 0)
			return 0;
		return memory / 1024;
	}

	// ATI cards
	if (epoxy_has_gl_extension("GL_ATI_meminfo")) {
		GLint memory[4];
		epoxy_glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, memory);
		return memory[0] / 1024;
	}

	// Intel cards share memory. We will approximate how much
	// Video ram is being used here based on system memory.
	// A maximum allocation of 1.7GB can be done on windows systems.
	// TOTOALLY NOT A VALID WAY OF DOING THIS. BUT WILL BE GOOD ENOUGH
	// FOR OUR PURPOSES.
	// We will assume max. allocation can be done if the system holds
	// 12 or more GB of physical ram.
	S32 systemRam = PlatformEx::getPhysicalSystemRam();
	if (systemRam >= 12288) // 12GB
		return 1700;
	if (systemRam >= 8192) // 8GB
		return 1536;
	if (systemRam >= 4096) // 4GB
		return 1024;
	if (systemRam >= 3072) // 3GB
		return 512;
	if (systemRam >= 2048) // 2GB
		return 256;
	if (systemRam >= 1024) // 1GB
		return 128;
	// Do we have a GPU? Is this a server? Is this PC from 1980?
	return 0;
#endif
}

void GL::setPixelDensityEXT(GLfloat pixelDensity) {
	if (mPixelDensity < 0.0f) {
		return;
	}
	mPixelDensity = pixelDensity;
}

GLfloat GL::getPixelDensityEXT() {
	return mPixelDensity;
}

std::string GL::translateShaderEXT(const std::string &source, GLenum type) {
	if (type == GL_VERTEX_SHADER)
		return mTranslator->translate(source, ShaderTranslator::ShaderType::VERTEX);
	if (type == GL_FRAGMENT_SHADER)
		return mTranslator->translate(source, ShaderTranslator::ShaderType::FRAGMENT);
	return "";
}