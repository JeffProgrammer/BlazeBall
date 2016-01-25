//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_PLATFORMGL_H_
#define _PLATFORM_PLATFORMGL_H_

#if defined(_WIN32)
#include <epoxy/gl.h>
#include <epoxy/wgl.h>
#elif defined(__linux__) || defined(__FREEBSD__)
#include <epoxy/gl.h>
#include <epoxy/wgl.h>
#elif defined(__APPLE__)
#include <OpenGL/gl3.h>
#endif

#endif
