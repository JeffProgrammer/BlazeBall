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
//
// SPECIAL NOTICE:
// Mouse movement is under the MIT license by gametutorials.com:
// http://www.gametutorials.com/tutorial/camera-part3-opengl-4/
//
// Copyright © 2014 GameTutorials
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//------------------------------------------------------------------------------

#include "platformGLFW/GLFWWindow.h"
#include "platformGLFW/GLFWEvent.h"

bool GLFWWindow::createContext() {
	//Init GLFW
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;

	// Try using the core profile first.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_CONFIG_CORE_MAJOR_GL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_CONFIG_CORE_MINOR_GL_VERSION);

	// set various other window hints
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);

	bool usingCoreGL = true;

	mWindow = glfwCreateWindow(1280, 720, "DIF Viewer Game", NULL, NULL);
	if (mWindow == NULL) {
		printf("A Core OpenGL profile has failed. Attempting to create a legacy profile.\n");
		usingCoreGL = false;

		// Fall back to legacy profile if the core opengl profile fails on this platform.
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_CONFIG_LEGACY_MAJOR_GL_VERSION);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_CONFIG_LEGACY_MINOR_GL_VERSION);

		mWindow = glfwCreateWindow(1280, 720, "DIF Viewer Game", NULL, NULL);
		if (mWindow == NULL) {
			printf("Unable to load a valid OpenGL context. Please make sure your drivers are up to date.\n");
			printf("OpenGL %d.%d is required.\n", GLFW_CONFIG_LEGACY_MAJOR_GL_VERSION, GLFW_CONFIG_LEGACY_MINOR_GL_VERSION);
			return false;
		}

		printf("Created a legacy OpenGL profile successfully.\n");
	}

	// make the context current.
	glfwMakeContextCurrent(mWindow);

#ifdef _WIN32
	// Warning: glewExperimental makes all runtime GLEW_extension checks invalid.
	// Have to write our own method to check if an extension exists at runtime.
	if (usingCoreGL)
		glewExperimental = true;
	GLenum errorrrrrrrrrrrrrr = glewInit();
	if (errorrrrrrrrrrrrrr) {
		printf("%s", glewGetErrorString(errorrrrrrrrrrrrrr));
		return false;
	}
#endif

	// store this instance as the user pointer for the window.
	// this is used for the input queue.
	glfwSetWindowUserPointer(mWindow, this);

	//Use Vsync
	glfwSwapInterval(1);

	// this prevents key states being lost within the same poll "frame"
	glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(mWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);

	// set up various input and callbacks
	glfwSetKeyCallback(mWindow, glfw_key_callback);
	glfwSetWindowFocusCallback(mWindow, glfw_window_focus_callback);

	//Lock cursor
	// this will also initialize just like the tutorial says to do
	lockCursor(true);

	return true;
}

void GLFWWindow::destroyContext() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void GLFWWindow::swapBuffers() {
	glfwSwapBuffers(mWindow);
}

void GLFWWindow::lockCursor(const bool &locked) {
	if (locked) {
		if (mCursorLocked) {
			printf("Cursor is already locked!");
			return;
		}

		// store the mouse position so that we can use it whenever
		// we restore it
		glfwGetCursorPos(mWindow, &mLockedPos.x, &mLockedPos.y);
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// now, set its position to 0, as we are following the tutorial
		// for mouse delta
		glfwSetCursorPos(mWindow, 0.0, 0.0);
	} else {
		if (!mCursorLocked) {
			printf("Cursor is already unlocked!");
			return;
		}

		// restore mouse position before we locked the mouse
		glfwSetCursorPos(mWindow, mLockedPos.x, mLockedPos.y);
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// store locked status
	mCursorLocked = locked;
}

glm::ivec2 GLFWWindow::getWindowSize() {
	S32 screenWidth, screenHeight;
	glfwGetFramebufferSize(mWindow, &screenWidth, &screenHeight);
	return glm::ivec2(screenWidth, screenHeight);
}

bool GLFWWindow::pollEvents(Event *&event) {
	if (mEventQueue.empty()) {
		// This will make glfw poll all events and call the desired callbacks.
		//
		// If there are no events, then this is the first time pollEvents is called
		// because of this, we can make glfw get the events for us.
		//
		// To prevent polling from being called within the same "frame", see the
		// section down where "A bit of a Hack:" is mentioned in this function.
		glfwPollEvents();

		// We always have to check if we should dispatch a quit event or not.
		if (glfwWindowShouldClose(mWindow)) {
			Event *quitEvent = new QuitEvent();
			mEventQueue.push(quitEvent);
		}

		// check for mouse movement if we are in "game mode" which means the cursor
		// is locked and we are playing.
		if (mCursorLocked)
			handleMouseMovement();
	}

	// If there are no events pushed into the queue, then no events had been
	// polled this "frame".
	if (mEventQueue.empty())
		return false;

	// Grab the event and pop it off of the front of the queue.
	event = mEventQueue.front();
	mEventQueue.pop();

	// A bit of a Hack:
	// If this is the last item we popped off, we are done here.
	// This is to prevent re-entering the function and polling for events again.
	if (mEventQueue.empty())
		return false;

	return true;
}

/// This is only called whenever the mouse is locked to grab mouse movement delta
/// for the camera. Please note that the delta and position will be the same
/// as we reset the position to 0,0 after getting the position.
void GLFWWindow::handleMouseMovement() {
	// grab delta. If the mouse position is 0,0, then it didn't move, 
	// so we don't dispatch events.
	MousePos pos;
	glfwGetCursorPos(mWindow, &pos.x, &pos.y);

	// there is movement, dispatch the event
	if (pos.x != 0.0 || pos.y != 0.0) {
		S32 x = static_cast<S32>(pos.x);
		S32 y = static_cast<S32>(pos.y);

		MouseMoveEvent *event = new MouseMoveEvent();
		event->position = glm::ivec2(x, y);
		event->delta = glm::ivec2(x, y);
		mEventQueue.push(event);
	}

	// reset for next "frame" to track delta
	// as per the tutorial's suggested route
	glfwSetCursorPos(mWindow, 0.0, 0.0);
}