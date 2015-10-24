//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "platformSDL/SDLEvent.h"

Event *SDLEvent::convert(SDL_Event sdlevent) {
	Event::Type type = convertType(sdlevent);

	switch (type) {
		case Event::Quit: return new QuitEvent();
		case Event::KeyDown: {
			KeyDownEvent *event = new KeyDownEvent();
			event->key = sdlevent.key.keysym.sym;
			event->modifier = sdlevent.key.keysym.mod;
			return event;
		}
		case Event::KeyUp: {
			KeyUpEvent *event = new KeyUpEvent();
			event->key = sdlevent.key.keysym.sym;
			event->modifier = sdlevent.key.keysym.mod;
			return event;
		}
		case Event::MouseDown: {
			MouseDownEvent *event = new MouseDownEvent();
			event->button = sdlevent.button.button;
			event->position = glm::ivec2(sdlevent.button.x, sdlevent.button.y);
			return event;
		}
		case Event::MouseUp: {
			MouseUpEvent *event = new MouseUpEvent();
			event->button = sdlevent.button.button;
			event->position = glm::ivec2(sdlevent.button.x, sdlevent.button.y);
			return event;
		}
		case Event::MouseMove: {
			MouseMoveEvent *event = new MouseMoveEvent();
			event->position = glm::ivec2(sdlevent.motion.x, sdlevent.motion.y);
			event->delta = glm::ivec2(sdlevent.motion.xrel, sdlevent.motion.yrel);
			return event;
		}
		case Event::WindowFocus: return new WindowFocusEvent();
		case Event::WindowBlur: return new WindowBlurEvent();
		case Event::WindowResize: {
			WindowResizeEvent *event = new WindowResizeEvent();
			event->newSize = glm::ivec2(sdlevent.window.data1, sdlevent.window.data2);
			return event;
		}
		default:
            return NULL;
	}
}