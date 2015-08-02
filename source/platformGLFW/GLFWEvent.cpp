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

#include "platformGLFW/GLFWEvent.h"
#include "platformGLFW/GLFWWindow.h"

void glfw_key_callback(GLFWwindow *window, S32 key, S32 scanCode, S32 action, S32 modifier) {
	// Get the window so we can get the queue to push events into.
	GLFWWindow *theWindow = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		KeyDownEvent *event = new KeyDownEvent();
		event->key = glfw_key_to_platform(key);
		event->modifier = glfw_key_modifier_to_platform(modifier);
		theWindow->pushEvent(event);
	} else if (action == GLFW_RELEASE) {
		KeyUpEvent *event = new KeyUpEvent();
		event->key = glfw_key_to_platform(key);
		event->modifier = glfw_key_modifier_to_platform(modifier);
		theWindow->pushEvent(event);
	}
}

void glfw_window_focus_callback(GLFWwindow *window, S32 focused) {
	// Get the window so we can get the queue to push events into.
	GLFWWindow *theWindow = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

	if (focused) {
		WindowFocusEvent *event = new WindowFocusEvent();
		theWindow->pushEvent(event);
	} else {
		WindowBlurEvent *event = new WindowBlurEvent();
		theWindow->pushEvent(event);
	}
}

KeyEvent::Key glfw_key_to_platform(S32 key) {
	switch (key) {
		// letters
		case GLFW_KEY_A: return KeyEvent::Key::KEY_A;
		case GLFW_KEY_B: return KeyEvent::Key::KEY_B;
		case GLFW_KEY_C: return KeyEvent::Key::KEY_C;
		case GLFW_KEY_D: return KeyEvent::Key::KEY_D;
		case GLFW_KEY_E: return KeyEvent::Key::KEY_E;
		case GLFW_KEY_F: return KeyEvent::Key::KEY_F;
		case GLFW_KEY_G: return KeyEvent::Key::KEY_G;
		case GLFW_KEY_H: return KeyEvent::Key::KEY_H;
		case GLFW_KEY_I: return KeyEvent::Key::KEY_I;
		case GLFW_KEY_J: return KeyEvent::Key::KEY_J;
		case GLFW_KEY_K: return KeyEvent::Key::KEY_K;
		case GLFW_KEY_L: return KeyEvent::Key::KEY_L;
		case GLFW_KEY_M: return KeyEvent::Key::KEY_M;
		case GLFW_KEY_N: return KeyEvent::Key::KEY_N;
		case GLFW_KEY_O: return KeyEvent::Key::KEY_O;
		case GLFW_KEY_P: return KeyEvent::Key::KEY_P;
		case GLFW_KEY_Q: return KeyEvent::Key::KEY_Q;
		case GLFW_KEY_R: return KeyEvent::Key::KEY_R;
		case GLFW_KEY_S: return KeyEvent::Key::KEY_S;
		case GLFW_KEY_T: return KeyEvent::Key::KEY_T;
		case GLFW_KEY_U: return KeyEvent::Key::KEY_U;
		case GLFW_KEY_V: return KeyEvent::Key::KEY_V;
		case GLFW_KEY_W: return KeyEvent::Key::KEY_W;
		case GLFW_KEY_X: return KeyEvent::Key::KEY_X;
		case GLFW_KEY_Y: return KeyEvent::Key::KEY_Y;
		case GLFW_KEY_Z: return KeyEvent::Key::KEY_Z;

		// Arrow keys
		case GLFW_KEY_UP: return KeyEvent::Key::KEY_UP;
		case GLFW_KEY_DOWN: return KeyEvent::Key::KEY_DOWN;
		case GLFW_KEY_LEFT: return KeyEvent::Key::KEY_LEFT;
		case GLFW_KEY_RIGHT: return KeyEvent::Key::KEY_RIGHT;

		// space
		case GLFW_KEY_SPACE: return KeyEvent::Key::KEY_SPACE;

		// Numbers
		case GLFW_KEY_0: return KeyEvent::Key::KEY_0;
		case GLFW_KEY_1: return KeyEvent::Key::KEY_1;
		case GLFW_KEY_2: return KeyEvent::Key::KEY_2;
		case GLFW_KEY_3: return KeyEvent::Key::KEY_3;
		case GLFW_KEY_4: return KeyEvent::Key::KEY_4;
		case GLFW_KEY_5: return KeyEvent::Key::KEY_5;
		case GLFW_KEY_6: return KeyEvent::Key::KEY_6;
		case GLFW_KEY_7: return KeyEvent::Key::KEY_7;
		case GLFW_KEY_8: return KeyEvent::Key::KEY_8;
		case GLFW_KEY_9: return KeyEvent::Key::KEY_9;

		// non-mapped key
		default: return KeyEvent::Key::KEY_INVALID_KEY;
	}
}

KeyEvent::Modifier glfw_key_modifier_to_platform(S32 modifier) {
	switch (modifier) {
	case GLFW_MOD_SHIFT: return KeyEvent::Modifier::MODIFIER_SHIFT;
	case GLFW_MOD_CONTROL: return KeyEvent::Modifier::MODIFIER_CONTROL;
	case GLFW_MOD_ALT: return KeyEvent::Modifier::MODIFIER_ALT;

		// no modifier
	default: return KeyEvent::Modifier::MODIFIER_NONE;
	}
}