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

#include "platformSDL/SDLWindow.h"
#include "platformSDL/SDLEvent.h"

bool SDLWindow::createContext() {
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		fprintf(stderr, "SDL Error: %s", SDL_GetError());
		return false;
	}

	// Try using the core profile first.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, SDL_CONFIG_MAJOR_GL_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, SDL_CONFIG_MINOR_GL_VERSION);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// set various other window hints
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Create the window
	const U32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
	if ((window = SDL_CreateWindow("DIF Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, flags)) == NULL)
		return false;

	// create context
	context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		printf("Unable to load a valid OpenGL context. Please make sure your drivers are up to date.\n");
		printf("OpenGL %d.%d is required.\n", SDL_CONFIG_MAJOR_GL_VERSION, SDL_CONFIG_MINOR_GL_VERSION);
		return false;
	}

	SDL_GL_MakeCurrent(window, context);

#ifdef _WIN32
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "GLEW failed to init. Error: %d\n", error);
		return false;
	}

	// stew on the error, as GLEW actually generates an error in teh core profile.
	// its known, and documented, on their github. and they refuse to do anything about it.
	// pussies.
	//
	// Uncomment me if we are using the opengl core profile
	//while (glGetError() != GL_NO_ERROR);
#endif

	printf("OpenGL Info\n");
	printf("    Version: %s\n", glGetString(GL_VERSION));
	printf("     Vendor: %s\n", glGetString(GL_VENDOR));
	printf("   Renderer: %s\n", glGetString(GL_RENDERER));
	printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Use Vsync
	setVerticalSync(true);
	
	//Lock cursor
	lockCursor(true);

	return true;
}

void SDLWindow::destroyContext() {
	SDL_Quit();
}

void SDLWindow::swapBuffers() {
	SDL_GL_SwapWindow(window);
}

void SDLWindow::lockCursor(const bool &locked) {
	SDL_SetRelativeMouseMode((SDL_bool)locked);
}

void SDLWindow::setWindowTitle(const char *title) {
	SDL_SetWindowTitle(window, title);
}

void SDLWindow::setVerticalSync(bool vsync) {
	mVsync = vsync;
	SDL_GL_SetSwapInterval(vsync ? 1 : 0);
}

void SDLWindow::toggleVsync() {
	setVerticalSync(!mVsync);
}

glm::ivec2 SDLWindow::getWindowSize() {
	S32 screenWidth, screenHeight;
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	return glm::ivec2(screenWidth, screenHeight);
}

bool SDLWindow::pollEvents(Event *&event) {
	SDL_Event sdlevent;
	if (SDL_PollEvent(&sdlevent)) {
		event = SDLEvent::convert(sdlevent);
		return true;
	} else {
		return false;
	}
}