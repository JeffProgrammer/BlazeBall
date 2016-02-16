//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_PLATFORMWINDOW_H_
#define _PLATFORM_PLATFORMWINDOW_H_

#include "base/types.h"
#include "platform/platformEvent.h"

class PlatformWindow {
public:
	virtual ~PlatformWindow() {};

	virtual bool createContext() = 0;
	virtual void destroyContext() = 0;
	virtual void swapBuffers() = 0;
	virtual void lockCursor(const bool &locked) = 0;
	virtual void setWindowTitle(const char *title) = 0;
	virtual void setVerticalSync(bool vsync) = 0;
	virtual void toggleVsync() = 0;

	virtual glm::ivec2 getWindowSize() = 0;
	virtual bool pollEvents(PlatformEvent *&event) = 0;
};

#endif // _PLATFORM_PLATFORMWINDOW_H_