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

#ifndef _PLATFORM_SDL_SDLEVENT_H_
#define _PLATFORM_SDL_SDLEVENT_H_

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

	inline MouseButton convertMouse(U8 button) {
		switch (button) {
		case SDL_BUTTON_LEFT:
			return MouseButton::MOUSE_BUTTON_LEFT;
		case SDL_BUTTON_RIGHT:
			return MouseButton::MOUSE_BUTTON_RIGHT;
		case SDL_BUTTON_MIDDLE:
			return MouseButton::MOUSE_BUTTON_MIDDLE;
		default:
			return MouseButton::MOUSE_BUTTON_INVALID;
		}
	}
};

#endif // _PLATFORM_SDL_SDLEVENT_H_