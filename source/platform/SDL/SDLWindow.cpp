//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "platform/SDL/SDLWindow.h"
#include "platform/SDL/SDLEvent.h"
#include "graphics/GL21/gl21.h"
#include "graphics/GL33/gl33.h"
#include "main/gameState.h"

enum GLContext {
	INVALID = -1,
	LEGACY  = 0,
	CORE    = 1
};

static bool createGLContextAndWindow(GLContext context, SDL_Window *&sdlWindow, SDL_GLContext &sdlGLContext) {
	S32 major, minor;
	if (context == GLContext::CORE) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		major = SDL_CONFIG_MAJOR_GL_VERSION;
		minor = SDL_CONFIG_MINOR_GL_VERSION;
	} else {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0x0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		major = SDL_CONFIG_MAJOR_LEGACY_GL_VERSION;
		minor = SDL_CONFIG_MINOR_LEGACY_GL_VERSION;
	}

	// Set OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

	// set various other window hints
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create the window
	const U32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
	auto window = SDL_CreateWindow("DIF Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, flags);
	if (window == NULL)
		return false;

	// create context
	auto glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		IO::printf("Unable to load a valid OpenGL context. Please make sure your drivers are up to date.\n");
		IO::printf("OpenGL %d.%d is required.\n", major, minor);
		return false;
	}

	// return the window and glcontext
	sdlWindow = window;
	sdlGLContext = glContext;

	return true;
}

bool SDLWindow::createContext() {
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		fprintf(stderr, "SDL Error: %s", SDL_GetError());
		return false;
	}

	// Check to see if we are forcing the legacy profile.
	bool forceLegacy = false;
	for (auto cmd : GameState::gState->mCmdArgs) {
		if (strcasecmp(cmd.c_str(), "-legacygl") == 0) {
			forceLegacy = true;
			break;
		}
	}

	SDL_GLContext context;

	// First try the core profile, unless we specified the legacy profile
	// as a command line argument.
	bool success = false;
	GLContext profileChosen = GLContext::INVALID;
	if (!forceLegacy) {
		IO::printf("Attempting to create a core OpenGL context.");
		success = createGLContextAndWindow(GLContext::CORE, window, context);
		if (success)
			profileChosen = GLContext::CORE;
	}

	if (forceLegacy || !success) {
		if (!success)
			IO::printf("Failed to create a core OpenGL context. Attempting to create a legacy context.\n");
		if (forceLegacy)
			IO::printf("Forcing the legacy OpenGL context. Attempting to create a legacy context.\n");

		success = createGLContextAndWindow(GLContext::LEGACY, window, context);
		if (success)
			profileChosen = GLContext::LEGACY;
	}

	// If we still didn't have success, bail.
	if (!success) {
		IO::printf("Failed to create an OpenGL context. Make sure that you have at least OpenGL 2.1.\n");
		return false;
	}

	SDL_GL_MakeCurrent(window, context);
#ifdef _WIN32
	epoxy_handle_external_wglMakeCurrent();
#endif

	// Initialize the GL library
	if (profileChosen == GLContext::CORE)
		GL::createGL<GL33>();
	else
		GL::createGL<GL21>();

	// for the love of god please not software rendering.
#if defined(_WIN32) || defined(__APPLE__)
	{
		std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	#ifdef _WIN32
		if (renderer.find("Microsoft") != std::string::npos) {
	#else
		if (renderer.find("APPLE") != std::string::npos) {
	#endif
			IO::printf("Unable to create a hardware accelerated OpenGL driver. Please make sure that you have OpenGL drivers downloaded and they are up to date.\n");
			return false;
		}
	}
#endif
	
	// Let the GL library store this context.
	glBindContextEXT(context);

	IO::printf("OpenGL Core Profile Info\n");
	IO::printf("   Version:  %s\n", glGetString(GL_VERSION));
	IO::printf("   Vendor:   %s\n", glGetString(GL_VENDOR));
	IO::printf("   Renderer: %s\n", glGetString(GL_RENDERER));
	IO::printf("   Shading:  %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	IO::printf("   VRAM:     %uMB\n", glGetVideoRamEXT());

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