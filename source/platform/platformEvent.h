//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef event_h
#define event_h

#include "base/types.h"
#include "base/event.h"

enum MouseButton {
	MOUSE_BUTTON_INVALID = -1,
	MOUSE_BUTTON_LEFT = 1,
	MOUSE_BUTTON_MIDDLE = 2,
	MOUSE_BUTTON_RIGHT = 3
};

class PlatformEvent : public Event {
public:
	PlatformEvent(Type type) : Event(type) {};
};

class QuitEvent : public PlatformEvent {
public:
	QuitEvent() : PlatformEvent(Quit) {};
};

class KeyDownEvent : public PlatformEvent {
public:
	U32 key;
	U32 modifier;

	KeyDownEvent() : PlatformEvent(KeyDown), key(0), modifier(0) {};
	U32 getKey() {
		return key;
	}
	U32 getModifier() {
		return modifier;
	}
};

class KeyUpEvent : public PlatformEvent {
public:
	U32 key;
	U32 modifier;

	KeyUpEvent() : PlatformEvent(KeyUp), key(0), modifier(0) {};
	U32 getKey() {
		return key;
	}
	U32 getModifier() {
		return modifier;
	}
};

class MouseMoveEvent : public PlatformEvent {
public:
	glm::ivec2 position;
	glm::ivec2 delta;

	MouseMoveEvent() : PlatformEvent(MouseMove), position(glm::ivec2(0, 0)), delta(glm::ivec2(0, 0)) {};
	glm::ivec2 getPosition() {
		return position;
	}
	glm::ivec2 getDelta() {
		return delta;
	}
};

class MouseDownEvent : public PlatformEvent {
public:
	glm::ivec2 position;
	MouseButton button;

	MouseDownEvent() : PlatformEvent(MouseDown), position(glm::ivec2(0, 0)), button(MOUSE_BUTTON_LEFT) {};
	glm::ivec2 getPosition() {
		return position;
	}
	MouseButton getButton() {
		return button;
	}
};

class MouseUpEvent : public PlatformEvent {
public:
	glm::ivec2 position;
	MouseButton button;

	MouseUpEvent() : PlatformEvent(MouseUp), position(glm::ivec2(0, 0)), button(MOUSE_BUTTON_LEFT) {};
	glm::ivec2 getPosition() {
		return position;
	}
	MouseButton getButton() {
		return button;
	}
};

/*
 Key events shamelessly stolen from SDL2
 */
namespace KeyEvent {
	enum Key {
		KEY_A                  = 'a',
		KEY_B                  = 'b',
		KEY_C                  = 'c',
		KEY_D                  = 'd',
		KEY_E                  = 'e',
		KEY_F                  = 'f',
		KEY_G                  = 'g',
		KEY_H                  = 'h',
		KEY_I                  = 'i',
		KEY_J                  = 'j',
		KEY_K                  = 'k',
		KEY_L                  = 'l',
		KEY_M                  = 'm',
		KEY_N                  = 'n',
		KEY_O                  = 'o',
		KEY_P                  = 'p',
		KEY_Q                  = 'q',
		KEY_R                  = 'r',
		KEY_S                  = 's',
		KEY_T                  = 't',
		KEY_U                  = 'u',
		KEY_V                  = 'v',
		KEY_W                  = 'w',
		KEY_X                  = 'x',
		KEY_Y                  = 'y',
		KEY_Z                  = 'z',
		KEY_1                  = '1',
		KEY_2                  = '2',
		KEY_3                  = '3',
		KEY_4                  = '4',
		KEY_5                  = '5',
		KEY_6                  = '6',
		KEY_7                  = '7',
		KEY_8                  = '8',
		KEY_9                  = '9',
		KEY_0                  = '0',
		KEY_RETURN             = '\r',
		KEY_ESCAPE             = '\033',
		KEY_BACKSPACE          = '\b',
		KEY_TAB                = '\t',
		KEY_SPACE              = ' ',
		KEY_MINUS              = '-',
		KEY_EQUALS             = '=',
		KEY_LEFTBRACKET        = '[',
		KEY_RIGHTBRACKET       = ']',
		KEY_BACKSLASH          = '\\',
		KEY_SEMICOLON          = ':',
		KEY_APOSTROPHE         = '\'',
		KEY_GRAVE              = '`',
		KEY_COMMA              = ',',
		KEY_PERIOD             = '.',
		KEY_SLASH              = '/',
		KEY_F1                 = 58 | (1 << 30), //This is what SDL uses
		KEY_F2                 = 59 | (1 << 30),
		KEY_F3                 = 60 | (1 << 30),
		KEY_F4                 = 61 | (1 << 30),
		KEY_F5                 = 62 | (1 << 30),
		KEY_F6                 = 63 | (1 << 30),
		KEY_F7                 = 64 | (1 << 30),
		KEY_F8                 = 65 | (1 << 30),
		KEY_F9                 = 66 | (1 << 30),
		KEY_F10                = 67 | (1 << 30),
		KEY_F11                = 68 | (1 << 30),
		KEY_F12                = 69 | (1 << 30),
		KEY_PRINTSCREEN        = 70 | (1 << 30),
		KEY_SCROLLLOCK         = 71 | (1 << 30),
		KEY_PAUSE              = 72 | (1 << 30),
		KEY_INSERT             = 73 | (1 << 30),
		KEY_HOME               = 74 | (1 << 30),
		KEY_PAGEUP             = 75 | (1 << 30),
		KEY_DELETE             = '\177',
		KEY_END                = 77 | (1 << 30),
		KEY_PAGEDOWN           = 78 | (1 << 30),
		KEY_RIGHT              = 79 | (1 << 30),
		KEY_LEFT               = 80 | (1 << 30),
		KEY_DOWN               = 81 | (1 << 30),
		KEY_UP                 = 82 | (1 << 30),
		KEY_NUMLOCKCLEAR       = 83 | (1 << 30),
		KEY_KP_DIVIDE          = 84 | (1 << 30),
		KEY_KP_MULTIPLY        = 85 | (1 << 30),
		KEY_KP_MINUS           = 86 | (1 << 30),
		KEY_KP_PLUS            = 87 | (1 << 30),
		KEY_KP_ENTER           = 88 | (1 << 30),
		KEY_KP_1               = 89 | (1 << 30),
		KEY_KP_2               = 90 | (1 << 30),
		KEY_KP_3               = 91 | (1 << 30),
		KEY_KP_4               = 92 | (1 << 30),
		KEY_KP_5               = 93 | (1 << 30),
		KEY_KP_6               = 94 | (1 << 30),
		KEY_KP_7               = 95 | (1 << 30),
		KEY_KP_8               = 96 | (1 << 30),
		KEY_KP_9               = 97 | (1 << 30),
		KEY_KP_0               = 98 | (1 << 30),
		KEY_KP_PERIOD          = 99 | (1 << 30),
		KEY_APPLICATION        = 101 | (1 << 30),
		KEY_POWER              = 102 | (1 << 30),
		KEY_KP_EQUALS          = 103 | (1 << 30),
		KEY_F13                = 104 | (1 << 30),
		KEY_F14                = 105 | (1 << 30),
		KEY_F15                = 106 | (1 << 30),
		KEY_F16                = 107 | (1 << 30),
		KEY_F17                = 108 | (1 << 30),
		KEY_F18                = 109 | (1 << 30),
		KEY_F19                = 110 | (1 << 30),
		KEY_F20                = 111 | (1 << 30),
		KEY_F21                = 112 | (1 << 30),
		KEY_F22                = 113 | (1 << 30),
		KEY_F23                = 114 | (1 << 30),
		KEY_F24                = 115 | (1 << 30),
		KEY_EXECUTEF           = 116 | (1 << 30), // [JTH] KEY_EXECUTE conflicts with a windows header as that is a define.
		KEY_HELP               = 117 | (1 << 30),
		KEY_MENU               = 118 | (1 << 30),
		KEY_SELECT             = 119 | (1 << 30),
		KEY_STOP               = 120 | (1 << 30),
		KEY_AGAIN              = 121 | (1 << 30),
		KEY_UNDO               = 122 | (1 << 30),
		KEY_CUT                = 123 | (1 << 30),
		KEY_COPY               = 124 | (1 << 30),
		KEY_PASTE              = 125 | (1 << 30),
		KEY_FIND               = 126 | (1 << 30),
		KEY_MUTE               = 127 | (1 << 30),
		KEY_VOLUMEUP           = 128 | (1 << 30),
		KEY_VOLUMEDOWN         = 129 | (1 << 30),
		KEY_KP_COMMA           = 133 | (1 << 30),
		KEY_KP_EQUALSAS400     = 134 | (1 << 30),
		KEY_ALTERASE           = 153 | (1 << 30),
		KEY_SYSREQ             = 154 | (1 << 30),
		KEY_CANCEL             = 155 | (1 << 30),
		KEY_CLEAR              = 156 | (1 << 30),
		KEY_PRIOR              = 157 | (1 << 30),
		KEY_RETURN2            = 158 | (1 << 30),
		KEY_SEPARATOR          = 159 | (1 << 30),
		KEY_OUT                = 160 | (1 << 30),
		KEY_OPER               = 161 | (1 << 30),
		KEY_CLEARAGAIN         = 162 | (1 << 30),
		KEY_CRSEL              = 163 | (1 << 30),
		KEY_EXSEL              = 164 | (1 << 30),
		KEY_KP_00              = 176 | (1 << 30),
		KEY_KP_000             = 177 | (1 << 30),
		KEY_THOUSANDSSEPARATOR = 178 | (1 << 30),
		KEY_DECIMALSEPARATOR   = 179 | (1 << 30),
		KEY_CURRENCYUNIT       = 180 | (1 << 30),
		KEY_CURRENCYSUBUNIT    = 181 | (1 << 30),
		KEY_KP_LEFTPAREN       = 182 | (1 << 30),
		KEY_KP_RIGHTPAREN      = 183 | (1 << 30),
		KEY_KP_LEFTBRACE       = 184 | (1 << 30),
		KEY_KP_RIGHTBRACE      = 185 | (1 << 30),
		KEY_KP_TAB             = 186 | (1 << 30),
		KEY_KP_BACKSPACE       = 187 | (1 << 30),
		KEY_KP_A               = 188 | (1 << 30),
		KEY_KP_B               = 189 | (1 << 30),
		KEY_KP_C               = 190 | (1 << 30),
		KEY_KP_D               = 191 | (1 << 30),
		KEY_KP_E               = 192 | (1 << 30),
		KEY_KP_F               = 193 | (1 << 30),
		KEY_KP_XOR             = 194 | (1 << 30),
		KEY_KP_POWER           = 195 | (1 << 30),
		KEY_KP_PERCENT         = 196 | (1 << 30),
		KEY_KP_LESS            = 197 | (1 << 30),
		KEY_KP_GREATER         = 198 | (1 << 30),
		KEY_KP_AMPERSAND       = 199 | (1 << 30),
		KEY_KP_DBLAMPERSAND    = 200 | (1 << 30),
		KEY_KP_VERTICALBAR     = 201 | (1 << 30),
		KEY_KP_DBLVERTICALBAR  = 202 | (1 << 30),
		KEY_KP_COLON           = 203 | (1 << 30),
		KEY_KP_HASH            = 204 | (1 << 30),
		KEY_KP_SPACE           = 205 | (1 << 30),
		KEY_KP_AT              = 206 | (1 << 30),
		KEY_KP_EXCLAM          = 207 | (1 << 30),
		KEY_KP_MEMSTORE        = 208 | (1 << 30),
		KEY_KP_MEMRECALL       = 209 | (1 << 30),
		KEY_KP_MEMCLEAR        = 210 | (1 << 30),
		KEY_KP_MEMADD          = 211 | (1 << 30),
		KEY_KP_MEMSUBTRACT     = 212 | (1 << 30),
		KEY_KP_MEMMULTIPLY     = 213 | (1 << 30),
		KEY_KP_MEMDIVIDE       = 214 | (1 << 30),
		KEY_KP_PLUSMINUS       = 215 | (1 << 30),
		KEY_KP_CLEAR           = 216 | (1 << 30),
		KEY_KP_CLEARENTRY      = 217 | (1 << 30),
		KEY_KP_BINARY          = 218 | (1 << 30),
		KEY_KP_OCTAL           = 219 | (1 << 30),
		KEY_KP_DECIMAL         = 220 | (1 << 30),
		KEY_KP_HEXADECIMAL     = 221 | (1 << 30),
		KEY_LCTRL              = 224 | (1 << 30),
		KEY_LSHIFT             = 225 | (1 << 30),
		KEY_LALT               = 226 | (1 << 30),
		KEY_LGUI               = 227 | (1 << 30),
		KEY_RCTRL              = 228 | (1 << 30),
		KEY_RSHIFT             = 229 | (1 << 30),
		KEY_RALT               = 230 | (1 << 30),
		KEY_RGUI               = 231 | (1 << 30),
		KEY_MODE               = 257 | (1 << 30),
		KEY_AUDIONEXT          = 258 | (1 << 30),
		KEY_AUDIOPREV          = 259 | (1 << 30),
		KEY_AUDIOSTOP          = 260 | (1 << 30),
		KEY_AUDIOPLAY          = 261 | (1 << 30),
		KEY_AUDIOMUTE          = 262 | (1 << 30),
		KEY_MEDIASELECT        = 263 | (1 << 30),
		KEY_WWW                = 264 | (1 << 30),
		KEY_MAIL               = 265 | (1 << 30),
		KEY_CALCULATOR         = 266 | (1 << 30),
		KEY_COMPUTER           = 267 | (1 << 30),
		KEY_AC_SEARCH          = 268 | (1 << 30),
		KEY_AC_HOME            = 269 | (1 << 30),
		KEY_AC_BACK            = 270 | (1 << 30),
		KEY_AC_FORWARD         = 271 | (1 << 30),
		KEY_AC_STOP            = 272 | (1 << 30),
		KEY_AC_REFRESH         = 273 | (1 << 30),
		KEY_AC_BOOKMARKS       = 274 | (1 << 30),
		KEY_BRIGHTNESSDOWN     = 275 | (1 << 30),
		KEY_BRIGHTNESSUP       = 276 | (1 << 30),
		KEY_DISPLAYSWITCH      = 277 | (1 << 30),
		KEY_KBDILLUMTOGGLE     = 278 | (1 << 30),
		KEY_KBDILLUMDOWN       = 279 | (1 << 30),
		KEY_KBDILLUMUP         = 280 | (1 << 30),
		KEY_EJECT              = 281 | (1 << 30),
		KEY_SLEEP              = 282 | (1 << 30)
	};
	enum Modifier {
		MOD_NONE     = 0x0000,
		MOD_LSHIFT   = 0x0001,
		MOD_RSHIFT   = 0x0002,
		MOD_LCTRL    = 0x0040,
		MOD_RCTRL    = 0x0080,
		MOD_LALT     = 0x0100,
		MOD_RALT     = 0x0200,
		MOD_LGUI     = 0x0400,
		MOD_RGUI     = 0x0800,
		MOD_NUM      = 0x1000,
		MOD_CAPS     = 0x2000,
		MOD_MODE     = 0x4000,
		MOD_RESERVED = 0x8000
	};
}

class WindowFocusEvent : public PlatformEvent {
public:
	WindowFocusEvent() : PlatformEvent(WindowFocus) {};
};

class WindowBlurEvent : public PlatformEvent {
public:
	WindowBlurEvent() : PlatformEvent(WindowBlur) {};
};

class WindowResizeEvent : public PlatformEvent {
public:
	glm::ivec2 newSize;
	WindowResizeEvent() : PlatformEvent(WindowResize), newSize(glm::ivec2(0, 0)) {};
};

#endif
