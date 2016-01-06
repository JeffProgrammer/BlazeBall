//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef SDLWindow_h
#define SDLWindow_h

#include <stdio.h>
#include "base/io.h"
#include "platform/platformWindow.h"
#include "platform/SDL/SDLConfig.h"

class SDLWindow : public PlatformWindow {
protected:
	SDL_Window *window;
	SDL_GLContext context;
	bool mVsync;
	GLuint mVAO;
	
public:
	SDLWindow() : window(nullptr), mVsync(false), mVAO(0) { }
	virtual ~SDLWindow() {};
	
	virtual bool createContext();
	virtual void destroyContext();
	virtual void swapBuffers();
	virtual void lockCursor(const bool &locked);
	virtual void setWindowTitle(const char *title);
	virtual void setVerticalSync(bool vsync);
	virtual void toggleVsync();

	virtual glm::ivec2 getWindowSize();
	virtual bool pollEvents(PlatformEvent *&event);
};

#endif
