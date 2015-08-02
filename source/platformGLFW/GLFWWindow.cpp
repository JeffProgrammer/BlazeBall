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

	mWindow = glfwCreateWindow(1280, 720, "DIF Viewer Game", NULL, NULL);
	if (mWindow == NULL) {
		printf("A Core OpenGL profile has failed. Attempting to create a legacy profile.\n");

		// Fall back to legacy profile if the core opengl profile fails on this platform.
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_CONFIG_LEGACY_MAJOR_GL_VERSION);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_CONFIG_LEGACY_MINOR_GL_VERSION);

		mWindow = glfwCreateWindow(1280, 720, "DIF Viewer Game", NULL, NULL);
		if (mWindow) {
			printf("Unable to load a valid OpenGL context. Please make sure your drivers are up to date.\n");
			printf("OpenGL %d.%d is required.\n", GLFW_CONFIG_LEGACY_MAJOR_GL_VERSION, GLFW_CONFIG_LEGACY_MINOR_GL_VERSION);
			return false;
		}

		printf("Created a legacy OpenGL profile successfully.\n");
	}

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
	if (locked)
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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