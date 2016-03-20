//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_SDL_SDLWINDOW_H_
#define _PLATFORM_SDL_SDLWINDOW_H_

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

#endif // _PLATFORM_SDL_SDLWINDOW_H_