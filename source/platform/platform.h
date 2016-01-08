//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_PLATFORM_H_
#define _PLATFORM_PLATFORM_H_

#include "platform/platformTimer.h"
#include "platform/platformWindow.h"

class Platform {
public:
	Platform() {}
	virtual ~Platform() {}

	virtual PlatformTimer *createTimer() = 0;
	virtual PlatformWindow *createWindow() = 0;
};

#endif
