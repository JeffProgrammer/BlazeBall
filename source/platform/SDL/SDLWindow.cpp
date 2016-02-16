//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "platform/SDL/SDLWindow.h"
#include "platform/SDL/SDLEvent.h"
#ifndef EMSCRIPTEN
#include "graphics/GL33/gl33.h"
#endif

bool SDLWindow::createContext() {
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		fprintf(stderr, "SDL Error: %s", SDL_GetError());
		return false;
	}

	// Try using the core profile first.
	// SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG will prevent deprecated functions
	// from working on on Windows and Linux [OSX already does this]
	// SDL_GL_CONTEXT_PROFILE_CORE will force the OpenGL core profile.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, SDL_CONFIG_MAJOR_GL_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, SDL_CONFIG_MINOR_GL_VERSION);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// set various other window hints
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create the window
	const U32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
	if ((window = SDL_CreateWindow("DIF Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, flags)) == NULL)
		return false;

	// create context
	context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		IO::printf("Unable to load a valid OpenGL context. Please make sure your drivers are up to date.\n");
		IO::printf("OpenGL %d.%d is required.\n", SDL_CONFIG_MAJOR_GL_VERSION, SDL_CONFIG_MINOR_GL_VERSION);
		return false;
	}

	SDL_GL_MakeCurrent(window, context);
#ifdef _WIN32
	epoxy_handle_external_wglMakeCurrent();
#endif
#ifndef EMSCRIPTEN
	// Initialize the GL library
	GL::createGL<GL33>();

	// Let the GL library store this context.
	glBindContextEXT(context);

	IO::printf("Please note that your GPU may support a higher GL version or newer extensions.\n");
	IO::printf("Extensions outside of the core may be used, but are not required.\n");
	IO::printf("OpenGL Core Profile Info\n");
	IO::printf("   Version:  %s\n", glGetString(GL_VERSION));
	IO::printf("   Vendor:   %s\n", glGetString(GL_VENDOR));
	IO::printf("   Renderer: %s\n", glGetString(GL_RENDERER));
	IO::printf("   Shading:  %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	IO::printf("   VRAM:     %uMB\n", glGetVideoRamEXT());
#endif

	//Use Vsync
	setVerticalSync(true);
	
	//Lock cursor
	lockCursor(true);

	// we bind a blank VAO, as it's required for core profile
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	return true;
}

void SDLWindow::destroyContext() {
	// clean up VAO
	if (mVAO)
		glDeleteVertexArrays(1, &mVAO);
	SDL_GL_DeleteContext(context);
	glBindContextEXT(nullptr);
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

bool SDLWindow::pollEvents(PlatformEvent *&event) {
	SDL_Event sdlevent;
	if (SDL_PollEvent(&sdlevent)) {
		event = SDLEvent::convert(sdlevent);
		return true;
	} else {
		return false;
	}
}