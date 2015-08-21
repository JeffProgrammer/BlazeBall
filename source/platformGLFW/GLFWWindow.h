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


#ifndef GLFWWindow_h
#define GLFWWindow_h

#include <stdio.h>
#include <queue>
#include "platform/window.h"
#include "platformGLFW/GLFWConfig.h"

class GLFWWindow : public Window {
protected:
	GLFWwindow *mWindow;

	std::queue<Event*> mEventQueue;

	bool mCursorLocked;

	struct MousePos {
		F64 x;
		F64 y;
	};
	MousePos mLockedPos;

public:
	GLFWWindow() : mWindow(nullptr), mCursorLocked(false) {
		mLockedPos.x = 0.0;
		mLockedPos.y = 0.0;
	}

	virtual bool createContext();
	virtual void destroyContext();
	virtual void swapBuffers();
	virtual void lockCursor(const bool &locked);

	virtual glm::ivec2 getWindowSize();
	virtual bool pollEvents(Event *&event);

	/// Pushes an event onto the event queue.
	/// called from various GLFW callbacks.
	/// @param event The event to push onto the queue.
	inline void pushEvent(Event *event) {
		mEventQueue.push(event);
	}

	void handleMouseMovement();
};

#endif
