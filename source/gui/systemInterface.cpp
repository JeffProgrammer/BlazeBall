//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
// 
// Some code taken from the SDL2 sample in LibRocket.
//
// Copyright (c) 2008-2010 Nuno Silva
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//------------------------------------------------------------------------------

#include "gui/systemInterface.h"
#include "platform/platform.h"
#include "platform/platformEvent.h"
#include "main/gameState.h"

GuiInterface::GuiInterface() {
	mTimer = GameState::gState->platform->createTimer();
	mTimer->start();
}

GuiInterface::~GuiInterface() {
	delete mTimer;
}

Rocket::Core::Input::KeyIdentifier GuiInterface::translateKey(KeyEvent::Key key) {
	switch (key) {
		case KeyEvent::KEY_1: return Rocket::Core::Input::KI_1;
		case KeyEvent::KEY_2: return Rocket::Core::Input::KI_2;
		case KeyEvent::KEY_3: return Rocket::Core::Input::KI_3;
		case KeyEvent::KEY_4: return Rocket::Core::Input::KI_4;
		case KeyEvent::KEY_5: return Rocket::Core::Input::KI_5;
		case KeyEvent::KEY_6: return Rocket::Core::Input::KI_6;
		case KeyEvent::KEY_7: return Rocket::Core::Input::KI_7;
		case KeyEvent::KEY_8: return Rocket::Core::Input::KI_8;
		case KeyEvent::KEY_9: return Rocket::Core::Input::KI_9;
		case KeyEvent::KEY_B: return Rocket::Core::Input::KI_B;
		case KeyEvent::KEY_C: return Rocket::Core::Input::KI_C;
		case KeyEvent::KEY_D: return Rocket::Core::Input::KI_D;
		case KeyEvent::KEY_E: return Rocket::Core::Input::KI_E;
		case KeyEvent::KEY_F: return Rocket::Core::Input::KI_F;
		case KeyEvent::KEY_G: return Rocket::Core::Input::KI_G;
		case KeyEvent::KEY_H: return Rocket::Core::Input::KI_H;
		case KeyEvent::KEY_I: return Rocket::Core::Input::KI_I;
		case KeyEvent::KEY_J: return Rocket::Core::Input::KI_J;
		case KeyEvent::KEY_K: return Rocket::Core::Input::KI_K;
		case KeyEvent::KEY_L: return Rocket::Core::Input::KI_L;
		case KeyEvent::KEY_M: return Rocket::Core::Input::KI_M;
		case KeyEvent::KEY_N: return Rocket::Core::Input::KI_N;
		case KeyEvent::KEY_O: return Rocket::Core::Input::KI_O;
		case KeyEvent::KEY_P: return Rocket::Core::Input::KI_P;
		case KeyEvent::KEY_Q: return Rocket::Core::Input::KI_Q;
		case KeyEvent::KEY_R: return Rocket::Core::Input::KI_R;
		case KeyEvent::KEY_S: return Rocket::Core::Input::KI_S;
		case KeyEvent::KEY_T: return Rocket::Core::Input::KI_T;
		case KeyEvent::KEY_U: return Rocket::Core::Input::KI_U;
		case KeyEvent::KEY_V: return Rocket::Core::Input::KI_V;
		case KeyEvent::KEY_W: return Rocket::Core::Input::KI_W;
		case KeyEvent::KEY_X: return Rocket::Core::Input::KI_X;
		case KeyEvent::KEY_Y: return Rocket::Core::Input::KI_Y;
		case KeyEvent::KEY_Z: return Rocket::Core::Input::KI_Z;
		case KeyEvent::KEY_KP_COMMA: return Rocket::Core::Input::KI_OEM_COMMA;
		case KeyEvent::KEY_KP_1: return Rocket::Core::Input::KI_NUMPAD1;
		case KeyEvent::KEY_KP_2: return Rocket::Core::Input::KI_NUMPAD2;
		case KeyEvent::KEY_KP_3: return Rocket::Core::Input::KI_NUMPAD3;
		case KeyEvent::KEY_KP_4: return Rocket::Core::Input::KI_NUMPAD4;
		case KeyEvent::KEY_KP_5: return Rocket::Core::Input::KI_NUMPAD5;
		case KeyEvent::KEY_KP_6: return Rocket::Core::Input::KI_NUMPAD6;
		case KeyEvent::KEY_KP_7: return Rocket::Core::Input::KI_NUMPAD7;
		case KeyEvent::KEY_KP_8: return Rocket::Core::Input::KI_NUMPAD8;
		case KeyEvent::KEY_KP_9: return Rocket::Core::Input::KI_NUMPAD9;
		case KeyEvent::KEY_KP_ENTER: return Rocket::Core::Input::KI_NUMPADENTER;
		case KeyEvent::KEY_KP_MULTIPLY: return Rocket::Core::Input::KI_MULTIPLY;
		case KeyEvent::KEY_KP_PLUS: return Rocket::Core::Input::KI_ADD;
		case KeyEvent::KEY_SEPARATOR: return Rocket::Core::Input::KI_SEPARATOR;
		case KeyEvent::KEY_KP_MINUS: return Rocket::Core::Input::KI_SUBTRACT;
		case KeyEvent::KEY_KP_DECIMAL: return Rocket::Core::Input::KI_DECIMAL;
		case KeyEvent::KEY_KP_DIVIDE: return Rocket::Core::Input::KI_DIVIDE;
		case KeyEvent::KEY_KP_EQUALS: return Rocket::Core::Input::KI_OEM_NEC_EQUAL;
		case KeyEvent::KEY_TAB: return Rocket::Core::Input::KI_TAB;
		case KeyEvent::KEY_RETURN: return Rocket::Core::Input::KI_RETURN;
		case KeyEvent::KEY_PAGEUP: return Rocket::Core::Input::KI_PRIOR;
		case KeyEvent::KEY_PAGEDOWN: return Rocket::Core::Input::KI_NEXT;
		case KeyEvent::KEY_END: return Rocket::Core::Input::KI_END;
		case KeyEvent::KEY_HOME: return Rocket::Core::Input::KI_HOME;
		case KeyEvent::KEY_LEFT: return Rocket::Core::Input::KI_LEFT;
		case KeyEvent::KEY_UP: return Rocket::Core::Input::KI_UP;
		case KeyEvent::KEY_RIGHT: return Rocket::Core::Input::KI_RIGHT;
		case KeyEvent::KEY_DOWN: return Rocket::Core::Input::KI_DOWN;
		case KeyEvent::KEY_SELECT: return Rocket::Core::Input::KI_SELECT;
		case KeyEvent::KEY_PRINTSCREEN: return Rocket::Core::Input::KI_PRINT;
		case KeyEvent::KEY_EXECUTEF: return Rocket::Core::Input::KI_EXECUTE;
		case KeyEvent::KEY_INSERT: return Rocket::Core::Input::KI_INSERT;
		case KeyEvent::KEY_DELETE: return Rocket::Core::Input::KI_DELETE;
		case KeyEvent::KEY_HELP: return Rocket::Core::Input::KI_HELP;
		case KeyEvent::KEY_APPLICATION: return Rocket::Core::Input::KI_APPS;
		case KeyEvent::KEY_SLEEP: return Rocket::Core::Input::KI_SLEEP;
		case KeyEvent::KEY_F2: return Rocket::Core::Input::KI_F2;
		case KeyEvent::KEY_F3: return Rocket::Core::Input::KI_F3;
		case KeyEvent::KEY_F4: return Rocket::Core::Input::KI_F4;
		case KeyEvent::KEY_F5: return Rocket::Core::Input::KI_F5;
		case KeyEvent::KEY_F6: return Rocket::Core::Input::KI_F6;
		case KeyEvent::KEY_F7: return Rocket::Core::Input::KI_F7;
		case KeyEvent::KEY_F8: return Rocket::Core::Input::KI_F8;
		case KeyEvent::KEY_F9: return Rocket::Core::Input::KI_F9;
		case KeyEvent::KEY_F10: return Rocket::Core::Input::KI_F10;
		case KeyEvent::KEY_F11: return Rocket::Core::Input::KI_F11;
		case KeyEvent::KEY_F12: return Rocket::Core::Input::KI_F12;
		case KeyEvent::KEY_F13: return Rocket::Core::Input::KI_F13;
		case KeyEvent::KEY_F14: return Rocket::Core::Input::KI_F14;
		case KeyEvent::KEY_F15: return Rocket::Core::Input::KI_F15;
		case KeyEvent::KEY_F16: return Rocket::Core::Input::KI_F16;
		case KeyEvent::KEY_F17: return Rocket::Core::Input::KI_F17;
		case KeyEvent::KEY_F18: return Rocket::Core::Input::KI_F18;
		case KeyEvent::KEY_F19: return Rocket::Core::Input::KI_F19;
		case KeyEvent::KEY_F20: return Rocket::Core::Input::KI_F20;
		case KeyEvent::KEY_F21: return Rocket::Core::Input::KI_F21;
		case KeyEvent::KEY_F22: return Rocket::Core::Input::KI_F22;
		case KeyEvent::KEY_F23: return Rocket::Core::Input::KI_F23;
		case KeyEvent::KEY_F24: return Rocket::Core::Input::KI_F24;
		case KeyEvent::KEY_ESCAPE: return Rocket::Core::Input::KI_ESCAPE;
		case KeyEvent::KEY_SCROLLLOCK: return Rocket::Core::Input::KI_SCROLL;
		case KeyEvent::KEY_RSHIFT: return Rocket::Core::Input::KI_RSHIFT;
		case KeyEvent::KEY_LCTRL: return Rocket::Core::Input::KI_LCONTROL;
		case KeyEvent::KEY_RCTRL: return Rocket::Core::Input::KI_RCONTROL;
		case KeyEvent::KEY_MENU: return Rocket::Core::Input::KI_LMENU;
		case KeyEvent::KEY_AC_FORWARD: return Rocket::Core::Input::KI_BROWSER_FORWARD;
		case KeyEvent::KEY_AC_REFRESH: return Rocket::Core::Input::KI_BROWSER_REFRESH;
		case KeyEvent::KEY_AC_STOP: return Rocket::Core::Input::KI_BROWSER_STOP;
		case KeyEvent::KEY_AC_SEARCH: return Rocket::Core::Input::KI_BROWSER_SEARCH;
		case KeyEvent::KEY_AC_BOOKMARKS: return Rocket::Core::Input::KI_BROWSER_FAVORITES;
		case KeyEvent::KEY_AC_HOME: return Rocket::Core::Input::KI_BROWSER_HOME;
		case KeyEvent::KEY_VOLUMEDOWN: return Rocket::Core::Input::KI_VOLUME_DOWN;
		case KeyEvent::KEY_VOLUMEUP: return Rocket::Core::Input::KI_VOLUME_UP;
		case KeyEvent::KEY_AUDIONEXT: return Rocket::Core::Input::KI_MEDIA_NEXT_TRACK;
		case KeyEvent::KEY_AUDIOPREV: return Rocket::Core::Input::KI_MEDIA_PREV_TRACK;
		case KeyEvent::KEY_AUDIOSTOP: return Rocket::Core::Input::KI_MEDIA_STOP;
		case KeyEvent::KEY_AUDIOPLAY: return Rocket::Core::Input::KI_MEDIA_PLAY_PAUSE;
		case KeyEvent::KEY_MAIL: return Rocket::Core::Input::KI_LAUNCH_MAIL;
		case KeyEvent::KEY_MEDIASELECT: return Rocket::Core::Input::KI_LAUNCH_MEDIA_SELECT;
		case KeyEvent::KEY_KP_00: return Rocket::Core::Input::KI_ICO_00;
		case KeyEvent::KEY_CRSEL: return Rocket::Core::Input::KI_CRSEL;
		case KeyEvent::KEY_EXSEL: return Rocket::Core::Input::KI_EXSEL;
		case KeyEvent::KEY_CLEAR: return Rocket::Core::Input::KI_OEM_CLEAR;
		default: return Rocket::Core::Input::KI_UNKNOWN;
	}
}

S32 GuiInterface::translateMouseButton(U8 button) {
	switch (button) {
		case MouseButton::MOUSE_BUTTON_LEFT:
			return 0;
		case MouseButton::MOUSE_BUTTON_RIGHT:
			return 1;
		case MouseButton::MOUSE_BUTTON_MIDDLE:
			return 2;
		default:
			return 3;
	}
}

F32 GuiInterface::GetElapsedTime() {
	return F32(mTimer->getElapsedTimeSinceStart());
}

bool GuiInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String &message) {
	std::string Type;

	switch (type)
	{
	case Rocket::Core::Log::LT_ALWAYS:
		Type = "[Always]";
		break;
	case Rocket::Core::Log::LT_ERROR:
		Type = "[Error]";
		break;
	case Rocket::Core::Log::LT_ASSERT:
		Type = "[Assert]";
		break;
	case Rocket::Core::Log::LT_WARNING:
		Type = "[Warning]";
		break;
	case Rocket::Core::Log::LT_INFO:
		Type = "[Info]";
		break;
	case Rocket::Core::Log::LT_DEBUG:
		Type = "[Debug]";
		break;
	case Rocket::Core::Log::LT_MAX:
		break;
	};

	IO::printf("%s - %s\n", Type.c_str(), message.CString());

	return true;
}