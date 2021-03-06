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

#include "platform/SDL/SDLEvent.h"

PlatformEvent *SDLEvent::convert(SDL_Event sdlevent) {
	PlatformEvent::Type type = convertType(sdlevent);

	switch (type) {
		case PlatformEvent::Quit: return new QuitEvent();
		case PlatformEvent::KeyDown: {
			KeyDownEvent *event = new KeyDownEvent();
			event->key = sdlevent.key.keysym.sym;
			event->modifier = sdlevent.key.keysym.mod;
			return event;
		}
		case PlatformEvent::KeyUp: {
			KeyUpEvent *event = new KeyUpEvent();
			event->key = sdlevent.key.keysym.sym;
			event->modifier = sdlevent.key.keysym.mod;
			return event;
		}
		case PlatformEvent::MouseDown: {
			MouseDownEvent *event = new MouseDownEvent();
			event->button = convertMouse(sdlevent.button.button);
			event->position = glm::ivec2(sdlevent.button.x, sdlevent.button.y);
			return event;
		}
		case PlatformEvent::MouseUp: {
			MouseUpEvent *event = new MouseUpEvent();
			event->button = convertMouse(sdlevent.button.button);
			event->position = glm::ivec2(sdlevent.button.x, sdlevent.button.y);
			return event;
		}
		case PlatformEvent::MouseMove: {
			MouseMoveEvent *event = new MouseMoveEvent();
			event->position = glm::ivec2(sdlevent.motion.x, sdlevent.motion.y);
			event->delta = glm::ivec2(sdlevent.motion.xrel, sdlevent.motion.yrel);
			return event;
		}
		case PlatformEvent::WindowFocus: return new WindowFocusEvent();
		case PlatformEvent::WindowBlur: return new WindowBlurEvent();
		case PlatformEvent::WindowResize: {
			WindowResizeEvent *event = new WindowResizeEvent();
			event->newSize = glm::ivec2(sdlevent.window.data1, sdlevent.window.data2);
			return event;
		}
		default:
            return NULL;
	}
}