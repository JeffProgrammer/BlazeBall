//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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