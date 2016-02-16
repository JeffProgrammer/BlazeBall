//-----------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeffrey Hutchinson
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _GRAPHICS_GL_GL_H_
#define _GRAPHICS_GL_GL_H_

#include <GLES3/gl3.h>

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

}

/**
 * Keeps a reference of the current context to the GL libery internally.
 * @param context The current opengl context bound to the GL.
 */
inline void glBindContextEXT(void *context) {
	
}

/**
 * Get's the amount of dedicated video ram available to the GL.
 * Note that dedicated video ram could be shared memory in iGPU systems.
 * @return the amount of video ram, in MegaBytes, that the GL has access to.
 */
inline GLuint glGetVideoRamEXT() {
	return 128*1024*1024;
}

/**
 * Set's the pixel density. This is used for high dpi and/or retina displays.
 * @param pixelDensity The density for high resolution.
 */
inline void glSetPixelDensityEXT(GLfloat pixelDensity) {

}

/**
 * Get's the pixel density of the GL.
 * @return the density.
 */
inline GLfloat glGetPixelDensityEXT() {
	return 1;
}

#endif // _GRAPHICS_GL_GL_H_