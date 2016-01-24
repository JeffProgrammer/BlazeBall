#define gl//----------------------------------------------------------------------------- Copyright ( 2014-2016 Glenn Smit Copyright ( 2014-2016 Jeff Hutchinso GL::sGL->
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_GL_GL_H_
#define _GRAPHICS_GL_GL_H_

#include "graphics/glImplementation.h"


#define glBindBuffer(target, buffer) GL::sGL->bindBuffer( buffer)
#define glBindFrameBuffer(target, frameBuffer) GL::sGL->bindFrameBuffer(target, frameBuffer)
#define glBindRenderBuffer(target, renderBuffer) GL::sGL->bindRenderBuffer(target, renderBuffer)
#define glBindTexture(target, texture) GL::sGL->bindTexture(target, texture)
#define glBindVertexArray(array) GL::sGL->bindVertexArray(array)

#define glClear(bitField) GL::sGL->clear(bitField)
#define glClearColor(red, green, blue, alpha) GL::sGL->clearColor(red, green, blue, alpha)
#define glCompileShader(shader) GL::sGL->compileShader(shader)
#define glCreateShader(shaderType) GL::sGL->createShader(shaderType)
#define glCreateProgram() GL::sGL->createProgram()

#define glDeleteBuffers(count, buffers) GL::sGL->deleteBuffers(count, buffers)
#define glDeleteFramebuffers(count, frameBuffers) GL::sGL->deleteFrameBuffers(count, frameBuffers)
#define glDeleteProgram(program) GL::sGL->deleteProgram(program)
#define glDeleteRenderbuffers(count, renderbuffers) GL::sGL->deleteRenderbuffers(count, renderbuffers)
#define glDeleteShader(shader) GL::sGL->deleteShader(shader)
#define glDeleteTextures(count, textures) GL::sGL->deleteTextures(count, textures)
#define glDeleteVertexArrays(count, array) GL::sGL->deleteVertexArrays(count, array)
#define glDepthFunc(func) GL::sGL->depthFunc(func)
#define glDisable(cap) GL::sGL->disable(cap)
#define glDisableVertexAttribArray(index) GL::sGL->disableVertexAttribArray(index)
#define glDrawArrays(mode, first, count) GL::sGL->drawArrays(mode, first, count)
#define glDrawElements(mode, count, type, indices); GL::sGL->drawElements(mode, count, type, indices)

#define glEnable(cap) GL::sGL->glEnable(cap)
#define glEnableVertexAttribArray(index) GL::sGL->enableVertexAttribArray(index)

#define glFramebufferRenderBuffer(target, attachment, renderbufferTarget, renderBuffer) GL::sGL->framebufferRenderBuffer(target, attachment, renderbufferTarget, renderBuffer)
#define glFramebufferTexture2D(target, attachment, textarget, texture, level) GL::sGL->framebufferTexture2D(target, attachment, textarget, texture, level)

#define glGenBuffers(count, buffers) GL::sGL->genBuffers(count, buffers)
#define glGenerateMipMap(target) GL::sGL->generateMipMap(target)
#define glGenFrameBuffers(count, frameBuffers) GL::sGL->genFrameBuffers(count, frameBuffers)
#define glGenRenderBuffers(count, renderBuffers) GL::sGL->genRenderBuffers(count, renderBuffers)
#define glGenTextures(count, textures) GL::sGL->genTextures(count, textures)
#define glGenVertexArrays(count, arrays) GL::sGL->genVertexArrays(count, arrays)
#define glGetError() GL::sGL->getError()
#define glGetIntegerv(pname, data) GL::sGL->getIntegerv(pname, data)
#define glGetProgramInfoLog(program, maxLength, length, infoLog) GL::sGL->getProgramInfoLog(program, maxLength, length, infoLog)
#define glGetProgramiv(program, pname, params) GL::sGL->getProgramiv(program, pname, params)
#define glGetShaderInfoLog(shader, maxLength, length, infoLog) GL::sGL->getShaderInfoLog(shader, maxLength, length, infoLog)
#define glGetShaderiv(shader, pname, params) GL::sGL->getShaderiv(shader, pname, params)
#define glGetUniformLocation(program, name) GL::sGL->getUniformLocation(program, name)

#define glLinkProgram(program) GL::sGL->linkProgram(program)

#define glPixelStorei(pname, param) GL::sGL->pixelStorei(pname, param)

#define glRenderbufferStorage(target, internalFormat, width, height) GL::sGL->renderBufferStorage(target, internalFormat, width, height)

#define glShaderSource(shader, count, string, length) GL::sGL->shaderSource(shader, count, string, length)

#define glTexImage2D(target, level, internalFormat, width, height, border, format, type, data) GL::sGL->texImage2D(target, level, internalFormat, width, height, border, fromat, type, data)
#define glTexParameteri(target, pname, param) GL::sGL->texParameteri(target, pname, param)

#define glUniform1f(location, v0) GL::sGL->uniform1f(location, v0)
#define glUniform1i(location, v0) GL::sGL->uniform1i(location, v0)
#define glUniform2fv(location, count, value) GL::sGL->uniform2fv(location, count, value)
#define glUniform3fv(location, count, value) GL::sGL->uniform3fv(location, count, value)
#define glUniform4fv(location, count, value) GL::sGL->uniform4fv(location, count, value)
#define glUniform2iv(location, count, value) GL::sGL->uniform2iv(location, count, value)
#define glUniform3iv(location, count, value) GL::sGL->uniform3iv(location, count, value)
#define glUniform4iv(location, count, value) GL::sGL->uniform4iv(location, count, value)
#define glUniformMatrix3fv(location, count, transpose, value) GL::sGL->uniformMatrix3fv(location, count, transpose, value)
#define glUniformMatrix4fv(location, count, transpose, value) GL::sGL->uniformMatrix4fv(location, count, transpose, value)
#define glUseProgram(program) GL::sGL->useProgram(program)

#define glVertexAttribPointer(index, size, type, normalized, stride, pointer) GL::sGL->vertexAttribPointer(index, type, normalized, stride, pointer)
#define glViewport(x, y, width, height) GL::sGL->viewport(x, y, width, height)

// Custom extensions
#define glGetViewportEXT(x, y, width, height) GL::sGL->getViewportEXT(x, y, width, height)

#endif // _GRAPHICS_GL_GL_H_