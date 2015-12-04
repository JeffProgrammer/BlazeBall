//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef SDLEvent_h
#define SDLEvent_h

#include <stdio.h>
#include "platform/event.h"
#include "platformSDL/SDLConfig.h"

namespace SDLEvent {
	Event *convert(SDL_Event event);

	inline Event::Type convertType(SDL_Event event) {
		SDL_EventType sdlType = (SDL_EventType)event.type;
		switch (sdlType) {
			case SDL_QUIT:    return Event::Quit;
			case SDL_KEYDOWN: return Event::KeyDown;
			case SDL_KEYUP:   return Event::KeyUp;
			case SDL_MOUSEBUTTONDOWN: return Event::MouseDown;
			case SDL_MOUSEBUTTONUP:   return Event::MouseUp;
			case SDL_MOUSEMOTION:     return Event::MouseMove;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_FOCUS_LOST:   return Event::WindowBlur;
					case SDL_WINDOWEVENT_FOCUS_GAINED: return Event::WindowFocus;
					case SDL_WINDOWEVENT_RESIZED:      return Event::WindowResize;
				}
			default: return Event::None;
		}
	}
};

#endif
