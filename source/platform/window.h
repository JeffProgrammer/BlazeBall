//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef window_h
#define window_h

#include "base/types.h"
#include "platform/event.h"

class Window {
public:
	virtual ~Window() {};

	virtual bool createContext() = 0;
	virtual void destroyContext() = 0;
	virtual void swapBuffers() = 0;
	virtual void lockCursor(const bool &locked) = 0;
	virtual void setWindowTitle(const char *title) = 0;
	virtual void setVerticalSync(bool vsync) = 0;
	virtual void toggleVsync() = 0;

	virtual glm::ivec2 getWindowSize() = 0;
	virtual bool pollEvents(Event *&event) = 0;
};

#endif
