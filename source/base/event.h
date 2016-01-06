//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_EVENT_H_
#define _BASE_EVENT_H_

class Event {
public:
	enum Type {
		None,
		Quit,
		KeyDown,
		KeyUp,
		MouseMove,
		MouseDown,
		MouseUp,
		WindowFocus,
		WindowBlur,
		WindowResize
	};

	Type type;

	Type getType() {
		return type;
	}
	Type getType() const {
		return type;
	}

	Event(Type type) : type(type) {
		
	}
};

#endif
