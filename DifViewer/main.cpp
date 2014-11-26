//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <libgen.h>
#include "io.h"
#include "types.h"
#include "dif.h"
#include "texture.h"

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

//TRIGGER WARNING: LOTS OF GLOBAL VARIABLES. I BET "NOBODY" WOULD LOVE THIS.

U32 gDifCount;
DIF **gDifs;

bool gRunning;

SDL_Window *gWindow;
SDL_GLContext gContext;
GLfloat gAngle;

float gYaw, gPitch;
glm::vec3 gCameraPosition;

glm::mat4x4 gProjectionMatrix, gModelviewMatrix;

static float gCameraSpeed = 0.3f;
static float gMovementSpeed = 0.2f;

bool captureMouse = false;
bool mouseButtons[3] = {false, false, false};
bool movement[4] = {false, false, false, false};

void render() {
	//Load the model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_CULL_FACE);

	//Camera
	gModelviewMatrix = glm::mat4x4(1);
	gModelviewMatrix = glm::rotate(gModelviewMatrix, gPitch, glm::vec3(1, 0, 0));
	gModelviewMatrix = glm::rotate(gModelviewMatrix, gYaw, glm::vec3(0, 1, 0));
	gModelviewMatrix = glm::translate(gModelviewMatrix, gCameraPosition);
	glLoadMatrixf(&gModelviewMatrix[0][0]);

	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, -1.0f);

	Point3F offset = {0.f, 0.f, 0.f};

	for (U32 index = 0; index < gDifCount; index ++) {
		for (U32 intIndex = 0; intIndex < gDifs[index]->numDetailLevels; intIndex ++) {
			interior_render(gDifs[index]->interior[intIndex], offset);
		}
	}

	glDisable(GL_CULL_FACE);
}

void loop() {
	//Basic movement
	glm::mat4x4 delta = glm::mat4x4(1);

	delta = glm::rotate(delta, -gYaw, glm::vec3(0, 1, 0));
	delta = glm::rotate(delta, -gPitch, glm::vec3(1, 0, 0));

	float speed = gMovementSpeed;
	if (mouseButtons[1])
		speed *= 2.f;

	if (movement[0]) delta = glm::translate(delta, glm::vec3(0, 0, speed));
	if (movement[1]) delta = glm::translate(delta, glm::vec3(0, 0, -speed));
	if (movement[2]) delta = glm::translate(delta, glm::vec3(speed, 0, 0));
	if (movement[3]) delta = glm::translate(delta, glm::vec3(-speed, 0, 0));

	gCameraPosition += glm::vec3(delta[3]);
}

bool initGL() {
	//Initialize clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClearDepth(1.0f);

	//Enable and set some shit for rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Window size for viewport
	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);

	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w * 2, h * 2);

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gProjectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);
	glMultMatrixf(&gProjectionMatrix[0][0]);

	return glGetError() == GL_NO_ERROR;
}

void performClick(S32 mouseX, S32 mouseY) {
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gWindow, &screenWidth, &screenHeight);

	//http://antongerdelan.net/opengl/raycasting.html
	//(x, y) are in device coordinates. We need to convert that to model coords

	//Device coords -> normalized device coords
	float x = (((F32)mouseX * 2.0f) / (F32)screenWidth) - 1.0f;
	float y = 1.0f - (((F32)mouseY * 2.0f) / (F32)screenHeight);
	float z = 1.0f;
	glm::vec3 ndc = glm::vec3(x, y, z);

	//Normalized device coords -> clip coordinates
	glm::vec4 clip = glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);

	//Clip coordinates -> eye coordinates
	glm::vec4 eye = glm::inverse(gProjectionMatrix) * clip;
	eye = glm::vec4(eye.x, eye.y, -1.0f, 0.0f);

	//Eye coordinates -> modelview coordinates
	glm::vec3 world = glm::vec3(glm::inverse(gModelviewMatrix) * eye);

	RayF ray;
	ray.origin.x = -gCameraPosition.x;
	ray.origin.y = -gCameraPosition.y;
	ray.origin.z = -gCameraPosition.z;

	ray.direction.x = world.x;
	ray.direction.y = world.y;
	ray.direction.z = world.z;

	for (U32 i = 0; i < gDifCount; i ++) {
		DIF *dif = gDifs[i];
		for (U32 j = 0; j < dif->numDetailLevels; j ++) {
			Interior *interior = dif->interior[j];

			U32 surfaceNum = interior_ray_cast(interior, ray);
			if (surfaceNum != -1) {
				interior->surface[surfaceNum].textureIndex ++;
			}
		}
	}
}

void handleEvent(SDL_Event *event) {
	//Quit
	if (event->type == SDL_QUIT) {
		gRunning = false;
	}
	//Window resize
	if (event->type == SDL_WINDOWEVENT) {
		//Just reinit (lazy)
		initGL();
	}

	//Key events, movement
	if (event->type == SDL_KEYDOWN) {
		switch (((SDL_KeyboardEvent *)event)->keysym.scancode) {
			//Same for Colemak...
			case SDL_SCANCODE_W: movement[0] = true; break;
			case SDL_SCANCODE_S: movement[1] = true; break;
			case SDL_SCANCODE_A: movement[2] = true; break;
			case SDL_SCANCODE_D: movement[3] = true; break;
		}
	} else if (event->type == SDL_KEYUP) {
		switch (((SDL_KeyboardEvent *)event)->keysym.scancode) {
			case SDL_SCANCODE_W: movement[0] = false; break;
			case SDL_SCANCODE_S: movement[1] = false; break;
			case SDL_SCANCODE_A: movement[2] = false; break;
			case SDL_SCANCODE_D: movement[3] = false; break;
		}
	}
	//Mouse for rotation
	if (event->type == SDL_MOUSEMOTION) {
		if (mouseButtons[2]) {
			gYaw += (GLfloat)((SDL_MouseMotionEvent *)event)->xrel * gCameraSpeed;
			gPitch += (GLfloat)((SDL_MouseMotionEvent *)event)->yrel * gCameraSpeed;
		}
	}
	if (event->type == SDL_MOUSEBUTTONDOWN) {
		mouseButtons[((SDL_MouseButtonEvent *)event)->button - 1] = true;

		if (((SDL_MouseButtonEvent *)event)->button == 3) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		if (((SDL_MouseButtonEvent *)event)->button == 1) {
			performClick(((SDL_MouseButtonEvent *)event)->x, ((SDL_MouseButtonEvent *)event)->y);
		}

	} else if (event->type == SDL_MOUSEBUTTONUP) {
		mouseButtons[((SDL_MouseButtonEvent *)event)->button - 1] = false;

		if (((SDL_MouseButtonEvent *)event)->button == 3) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}
}

bool init() {
	gRunning = true;

	//Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	//Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_Rect bounds;
	SDL_GetDisplayBounds(0, &bounds);

	//Create the window
	if ((gWindow = SDL_CreateWindow("DIF Viewer", (bounds.w - 1280) / 2, (bounds.h - 720) / 2, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)) == NULL) {
		return false;
	}

	//Create context
	if ((gContext = SDL_GL_CreateContext(gWindow)) == NULL) {
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		return false;
	}

	//Lock cursor
	SDL_SetRelativeMouseMode(SDL_TRUE);
	captureMouse = true;

	//Initialize OpenGL
	if (!initGL()) {
		return false;
	}
	return true;
}

void cleanup() {
	//Destroy the SDL
	SDL_Quit();
}

void run() {
	//Init SDL
	if (!init()) {
		exit(-1);
	}

	SDL_Event event;

	//Main loop
	while (gRunning) {
		//Profiling
		struct timeval startTime, endTime;
		gettimeofday(&startTime, NULL);

		//Input
		while (SDL_PollEvent(&event)) {
			handleEvent(&event);
		}

		//Hard work
		loop();
		render();

		//Flip buffers
		SDL_GL_SwapWindow(gWindow);

		//Profiling
		gettimeofday(&endTime, NULL);

		long long start = startTime.tv_usec + (startTime.tv_sec * 1000000ULL);
		long long end = endTime.tv_usec + (endTime.tv_sec * 1000000ULL);

//		printf("%f FPS, %f mspf\n", (1000.f / ((double)(end - start) / 1000.0f)), ((double)(end - start) / 1000.0f));
	}

	//Clean up (duh)
	cleanup();
}

int main(int argc, const char * argv[])
{
	//Usage prompt
	if (argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	U32 argstart = 1;

	if (!strcmp(argv[1], "-o")) {
		argstart += 2;
	}

	gDifCount = 0;
	gDifs = (DIF **)malloc(sizeof(DIF *) * (argc - argstart));
	for (U32 i = 0; i < (argc - argstart); i ++) {
		String directory = (String)dirname((char *)argv[i + argstart]);

		//Open file
		FILE *file = fopen(argv[i + argstart], "r");

		//Read the .dif
		gDifs[i] = dif_read_file(file, directory);
		if (gDifs[i]) {
			gDifCount ++;
		}

		//Clean up
		fclose(file);
	}

	if (!strcmp(argv[1], "-o")) {
		FILE *test = fopen(argv[2], "w");
		interior_export_obj(gDifs[0]->interior[0], test);
		fclose(test);
	} else {
		//Init SDL and go!
		run();
	}

	//Clean up
	for (U32 i = 0; i < gDifCount; i ++) {
		dif_release(gDifs[i]);
	}
	free(gDifs);

	return 0;
}
