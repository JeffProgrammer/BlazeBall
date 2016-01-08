//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "SDLPlatform.h"
#include "platform/SDL/SDLTimer.h"
#include "platform/SDL/SDLWindow.h"

PlatformTimer *SDLPlatform::createTimer() {
	return new SDLTimer();
}

PlatformWindow *SDLPlatform::createWindow() {
	return new SDLWindow();
}
