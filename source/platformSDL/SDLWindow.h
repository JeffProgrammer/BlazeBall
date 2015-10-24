//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef SDLWindow_h
#define SDLWindow_h

#include <stdio.h>
#include "platform/window.h"
#include "platformSDL/SDLConfig.h"

class SDLWindow : public Window {
protected:
	SDL_Window *window;
	SDL_GLContext context;
	bool mVsync;
	
public:
	SDLWindow() : window(nullptr), mVsync(false) { }
	virtual ~SDLWindow() {};
	
	virtual bool createContext();
	virtual void destroyContext();
	virtual void swapBuffers();
	virtual void lockCursor(const bool &locked);
	virtual void setWindowTitle(const char *title);
	virtual void setVerticalSync(bool vsync);
	virtual void toggleVsync();

	virtual glm::ivec2 getWindowSize();
	virtual bool pollEvents(Event *&event);
};

#endif
