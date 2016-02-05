//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_EVENT_H_
#define _BASE_EVENT_H_

class Event {
public:
	enum Type : U32 {
		None,
		Quit,
		KeyDown,
		KeyUp,
		MouseMove,
		MouseDown,
		MouseUp,
		WindowFocus,
		WindowBlur,
		WindowResize,

		NetConnect,
		NetDisconnect,
		NetGhostCreate,
		NetGhostUpdate,
		NetGhostControlObject
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
	virtual ~Event() {

	}
};

#endif // _BASE_EVENT_H_