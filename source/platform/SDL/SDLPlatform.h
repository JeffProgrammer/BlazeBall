//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_SDL_SDLPLATFORM_H_
#define _PLATFORM_SDL_SDLPLATFORM_H_

#include "platform/platform.h"

class SDLPlatform : public Platform {
public:
	virtual PlatformTimer *createTimer();
	virtual PlatformWindow *createWindow();
};

#endif
