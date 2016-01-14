//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------


#ifndef _PLATFORMSDL_SDLCONFIG_H_
#define _PLATFORMSDL_SDLCONFIG_H_

#ifdef _WIN32
#include <GL/glew.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#elif EMSCRIPTEN
#include <GLES3/gl3.h>
#endif

#include <SDL.h>
#undef main

/// We are using SDL
#define USING_SDL

/// The major opengl version required
#define SDL_CONFIG_MAJOR_GL_VERSION 3

/// The minor opengl version required
#define SDL_CONFIG_MINOR_GL_VERSION 3

#endif // _PLATFORMSDL_SDLCONFIG_H_
