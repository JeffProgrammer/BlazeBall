//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef SDLEvent_h
#define SDLEvent_h

#include <stdio.h>
#include "platform/platformEvent.h"
#include "platform/SDL/SDLConfig.h"

namespace SDLEvent {
	PlatformEvent *convert(SDL_Event event);

	inline PlatformEvent::Type convertType(SDL_Event event) {
		SDL_EventType sdlType = (SDL_EventType)event.type;
		switch (sdlType) {
			case SDL_QUIT:    return PlatformEvent::Quit;
			case SDL_KEYDOWN: return PlatformEvent::KeyDown;
			case SDL_KEYUP:   return PlatformEvent::KeyUp;
			case SDL_MOUSEBUTTONDOWN: return PlatformEvent::MouseDown;
			case SDL_MOUSEBUTTONUP:   return PlatformEvent::MouseUp;
			case SDL_MOUSEMOTION:     return PlatformEvent::MouseMove;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_FOCUS_LOST:   return PlatformEvent::WindowBlur;
					case SDL_WINDOWEVENT_FOCUS_GAINED: return PlatformEvent::WindowFocus;
					case SDL_WINDOWEVENT_RESIZED:      return PlatformEvent::WindowResize;
				}
			default: return PlatformEvent::None;
		}
	}
};

#endif
