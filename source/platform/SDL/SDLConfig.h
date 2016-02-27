//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------


#ifndef _PLATFORM_SDL_SDLCONFIG_H_
#define _PLATFORM_SDL_SDLCONFIG_H_

#include "platform/platformGL.h"

#include <SDL.h>
#undef main

/// We are using SDL
#define USING_SDL

/// The major opengl version required
#define SDL_CONFIG_MAJOR_GL_VERSION 3

/// The minor opengl version required
#define SDL_CONFIG_MINOR_GL_VERSION 3

/// The major opengl legacy profile version required
#define SDL_CONFIG_MAJOR_LEGACY_GL_VERSION 2

/// The minor opengl legacy profile version required
#define SDL_CONFIG_MINOR_LEGACY_GL_VERSION 1

#endif // _PLATFORM_SDL_SDLCONFIG_H_
