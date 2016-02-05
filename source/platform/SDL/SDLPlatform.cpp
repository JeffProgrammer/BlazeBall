//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
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
