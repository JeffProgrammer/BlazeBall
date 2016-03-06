//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_PLATFORM_H_
#define _PLATFORM_PLATFORM_H_

#include <memory>
#include "platform/platformTimer.h"
#include "platform/platformWindow.h"
#include "physics/physicsEngine.h"

class Platform {
public:
	Platform() {}
	virtual ~Platform() {}

	virtual PlatformTimer *createTimer() = 0;
	virtual PlatformWindow *createWindow() = 0;
};

namespace PlatformEx {
	S32 getPhysicalSystemRam();
};

#endif // _PLATFORM_PLATFORM_H_