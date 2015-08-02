//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
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

#ifndef event_h
#define event_h

#include "base/types.h"

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
		WindowBlur
	};

	Type type;

	Type getType() {
		return type;
	}
	Type getType() const {
		return type;
	}
	Event(Type type) : type(type) {};
};

class QuitEvent : public Event {
public:
	QuitEvent() : Event(Quit) {};
};

class KeyDownEvent : public Event {
public:
	U16 key;
	U16 modifier;

	KeyDownEvent() : Event(KeyDown) {};
	U16 getKey() {
		return key;
	}
	U16 getModifier() {
		return modifier;
	}
};

class KeyUpEvent : public Event {
public:
	U16 key;
	U16 modifier;

	KeyUpEvent() : Event(KeyUp) {};
	U16 getKey() {
		return key;
	}
	U16 getModifier() {
		return modifier;
	}
};

class MouseMoveEvent : public Event {
public:
	glm::ivec2 position;
	glm::ivec2 delta;

	MouseMoveEvent() : Event(MouseMove) {};
	glm::ivec2 getPosition() {
		return position;
	}
	glm::ivec2 getDelta() {
		return delta;
	}
};

class MouseDownEvent : public Event {
public:
	glm::ivec2 position;
	U8 button;

	MouseDownEvent() : Event(MouseDown) {};
	glm::ivec2 getPosition() {
		return position;
	}
	U8 getButton() {
		return button;
	}
};

class MouseUpEvent : public Event {
public:
	glm::ivec2 position;
	U8 button;

	MouseUpEvent() : Event(MouseUp) {};
	glm::ivec2 getPosition() {
		return position;
	}
	U8 getButton() {
		return button;
	}
};

/*
  Platform agnostic Key events
 */
namespace KeyEvent {
	enum Key {
		KEY_INVALID_KEY = -1,

		// Letters
		KEY_A = 0,
		KEY_B = 1,
		KEY_C = 2,
		KEY_D = 3,
		KEY_E = 4,
		KEY_F = 5,
		KEY_G = 6,
		KEY_H = 7,
		KEY_I = 8,
		KEY_J = 9,
		KEY_K = 10,
		KEY_L = 11,
		KEY_M = 12,
		KEY_N = 13,
		KEY_O = 14,
		KEY_P = 15,
		KEY_Q = 16,
		KEY_R = 17,
		KEY_S = 18,
		KEY_T = 19,
		KEY_U = 20,
		KEY_V = 21,
		KEY_W = 22,
		KEY_X = 23,
		KEY_Y = 24,
		KEY_Z = 25,

		// Arrow keys
		KEY_UP = 26,
		KEY_DOWN = 27,
		KEY_LEFT = 28,
		KEY_RIGHT = 29,

		// space key
		KEY_SPACE = 30,

		// numbers
		KEY_0 = 31,
		KEY_1 = 32,
		KEY_2 = 33,
		KEY_3 = 34,
		KEY_4 = 35,
		KEY_5 = 36,
		KEY_6 = 37,
		KEY_7 = 38,
		KEY_8 = 39,
		KEY_9 = 40
	};

	enum Modifier {
		// modifiers
		MODIFIER_NONE = 0,
		MODIFIER_SHIFT = 1,
		MODIFIER_CONTROL = 2,
		MODIFIER_ALT = 3
	};
}

class WindowFocusEvent : public Event {
public:
	WindowFocusEvent() : Event(WindowFocus) {};
};

class WindowBlurEvent : public Event {
public:
	WindowBlurEvent() : Event(WindowBlur) {};
};

#endif
