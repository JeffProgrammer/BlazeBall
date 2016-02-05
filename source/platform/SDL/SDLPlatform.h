//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_SDL_SDLPLATFORM_H_
#define _PLATFORM_SDL_SDLPLATFORM_H_

#include "platform/platform.h"

class SDLPlatform : public Platform {
public:
	virtual PlatformTimer *createTimer();
	virtual PlatformWindow *createWindow();
	SDLPlatform(std::unique_ptr<PhysicsEngineFactory> &&physFactory) : Platform(std::move(physFactory)) {}
};

#endif // _PLATFORM_SDL_SDLPLATFORM_H_
