//------------------------------------------------------------------------------
// Copyright (C) 2014 Glenn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "io.h"
#include "types.h"
#include "dif.h"

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

//TRIGGER WARNING: LOTS OF GLOBAL VARIABLES. I BET "NOBODY" WOULD LOVE THIS.

U32 gDifCount;
DIF **gDifs;

U32 gTriangleCount;
Triangle *gTriangles;

bool gRunning;

SDL_Window *gWindow;
SDL_GLContext gContext;
GLfloat gAngle;

float gYaw, gPitch;
glm::vec3 gCameraPosition;

static float gCameraSpeed = 0.1f;
static float gMovementSpeed = 0.2f;

bool mouseButtons[3] = {false, false, false};
bool movement[4] = {false, false, false, false};

void render() {
	//Load the model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_CULL_FACE);

	//Camera
	glm::mat4x4 cameraMatrix = glm::mat4x4(1);
	cameraMatrix = glm::rotate(cameraMatrix, gPitch, glm::vec3(1, 0, 0));
	cameraMatrix = glm::rotate(cameraMatrix, gYaw, glm::vec3(0, 1, 0));
	cameraMatrix = glm::translate(cameraMatrix, gCameraPosition);
	glLoadMatrixf(&cameraMatrix[0][0]);

	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, -1.0f);

	//Actual rendering is here (GL 1.1 in a 2.1 context. Take THAT, good practice!)
	//TODO: VBOs
	glBegin(GL_TRIANGLES);
	for (U32 i = 0; i < gTriangleCount; i ++) {
		glColor4f(gTriangles[i].color.red, gTriangles[i].color.green, gTriangles[i].color.blue, gTriangles[i].color.alpha);
		glNormal3f(gTriangles[i].normal.x, gTriangles[i].normal.z, gTriangles[i].normal.y);

		//Lazy, also wrong because Torque swaps y/z
		glVertex3f(gTriangles[i].point0.x, gTriangles[i].point0.z, -gTriangles[i].point0.y);
		glVertex3f(gTriangles[i].point1.x, gTriangles[i].point1.z, -gTriangles[i].point1.y);
		glVertex3f(gTriangles[i].point2.x, gTriangles[i].point2.z, -gTriangles[i].point2.y);
	}
	glEnd();
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
	glClearColor(0.f, 0.f, 0.f, 1.f);
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
	glViewport(0, 0, w, h);

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, aspect, 0.1f, 1000.f);

	return glGetError() == GL_NO_ERROR;
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
	if (event->type == SDL_APP_DIDENTERFOREGROUND || event->type == SDL_MOUSEBUTTONDOWN) {
		//Lock cursor
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	if (event->type == SDL_APP_WILLENTERBACKGROUND) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	//Key events, movement
	if (event->type == SDL_KEYDOWN) {
		switch (((SDL_KeyboardEvent *)event)->keysym.scancode) {
			//Same for Colemak...
			case SDL_SCANCODE_W: movement[0] = true; break;
			case SDL_SCANCODE_S: movement[1] = true; break;
			case SDL_SCANCODE_A: movement[2] = true; break;
			case SDL_SCANCODE_D: movement[3] = true; break;
			case SDL_SCANCODE_ESCAPE:
				//Unlock cursor
				SDL_SetRelativeMouseMode(SDL_FALSE);
				break;
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
		gYaw += (GLfloat)((SDL_MouseMotionEvent *)event)->xrel * gCameraSpeed;
		gPitch += (GLfloat)((SDL_MouseMotionEvent *)event)->yrel * gCameraSpeed;
	}
	if (event->type == SDL_MOUSEBUTTONDOWN) {
		mouseButtons[((SDL_MouseButtonEvent *)event)->button] = true;
	} else if (event->type == SDL_MOUSEBUTTONUP) {
		mouseButtons[((SDL_MouseButtonEvent *)event)->button] = false;
	}
}

bool init() {
	//Load our map into triangles (global var warning)
	gTriangles = interior_generate_triangles(gDifs[0]->interior[0], &gTriangleCount);

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
	if ((gWindow = SDL_CreateWindow("DIF Viewer", (bounds.w - 1280) / 2, (bounds.h - 720) / 2, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL) {
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
		//Open file
		FILE *file = fopen(argv[i + argstart], "r");

		//Read the .dif
		gDifs[i] = dif_read_file(file);
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
