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

typedef struct {
	Point3F point0;
	Point3F point1;
	Point3F point2;
} Triangle;

//TRIGGER WARNING: LOTS OF GLOBAL VARIABLES. I BET "NOBODY" WOULD LOVE THIS.

U32 gDifCount;
DIF **gDifs;

U32 gTriangleCount;
Triangle *gTriangles;

bool gRunning;

SDL_Window *gWindow;
SDL_GLContext *gContext;
GLfloat gAngle;

GLfloat gCameraPos[3] = {0.f, 0.f, 0.f};
GLfloat gCameraRot[2] = {0.f, 0.f};
static float gCameraSpeed = 0.1f;
static float gMovementSpeed = 0.2f;

bool movement[4] = {false, false, false, false};

GLfloat *hsvToRGB(GLfloat h, GLfloat s, GLfloat v) {
	//https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
	//Lazy!

	GLfloat *rgb = malloc(sizeof(GLfloat) * 3);
	GLfloat c = v * s;
	GLint hp = (GLint)(h * 6.f);
	GLfloat x = c * (1.f - fabs(fmod((float)hp, 2.f) - 1.f));
	switch (hp) {
		case 0: rgb[0] = c; rgb[1] = x; rgb[2] = 0; break;
		case 1: rgb[0] = x; rgb[1] = c; rgb[2] = 0; break;
		case 2: rgb[0] = 0; rgb[1] = c; rgb[2] = x; break;
		case 3: rgb[0] = 0; rgb[1] = x; rgb[2] = c; break;
		case 4: rgb[0] = x; rgb[1] = 0; rgb[2] = c; break;
		case 5: rgb[0] = c; rgb[1] = 0; rgb[2] = x; break;
	}
	return rgb;
}

void generateTriangles() {
	gTriangleCount = 0;
	for (U32 dif = 0; dif < gDifCount; dif ++) {
		for (U32 level = 0; level < gDifs[dif]->numDetailLevels; level ++) {
			Interior *interior = gDifs[dif]->interior[level];

			for (U32 i = 0; i < interior->numSurfaces; i ++) {
				Surface surface = interior->surface[i];
				U8 windingCount = surface.windingCount;
				//Triangles = (points - 2)
				windingCount -= 2;
				gTriangleCount += windingCount;
			}
		}
	}

	gTriangles = malloc(sizeof(Triangle) * gTriangleCount);
	U32 triIndex = 0;

	//Geometry is structured as lists of windings (point indices)
	//Windings are in order, forming triangle strips from the points.
	//
	// It makes sense if you think about it.

	//Actual generation
	for (U32 dif = 0; dif < gDifCount; dif ++) {
		for (U32 i = 0; i < gDifs[dif]->numDetailLevels; i ++) {
			Interior *interior = gDifs[dif]->interior[i];

			for (U32 j = 0; j < interior->numSurfaces; j ++) {
				Surface surface = interior->surface[j];

				U32 windingStart = surface.windingStart;
				U8 windingCount = surface.windingCount;

				//Triangle strips, but not how we want them. Somehow. I don't know; this actually works though.
				windingCount -= 2;

				for (U32 k = windingStart; k < windingStart + windingCount; k ++) {
					//Build triangles
					gTriangles[triIndex].point0 = interior->point[interior->index[k + 0]];
					gTriangles[triIndex].point1 = interior->point[interior->index[k + 1]];
					gTriangles[triIndex].point2 = interior->point[interior->index[k + 2]];
					triIndex ++;
				}
			}
		}
	}
}

void render() {
	//Load the model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Camera
	glTranslatef(gCameraPos[0], gCameraPos[1], gCameraPos[2]);
	glRotatef(gCameraRot[0], 1.f, 0, 0);
	glRotatef(gCameraRot[1], 0, 1.f, 0);

	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, -1.0f);

	//Actual rendering is here (GL 1.1 in a 2.1 context. Take THAT, good practice!)
	//TODO: VBOs
	glBegin(GL_TRIANGLES);
	for (U32 i = 0; i < gTriangleCount; i ++) {
		//Triangle-based color (probably)
		GLfloat *rgb = hsvToRGB((GLfloat)i / (GLfloat)gTriangleCount, 1.f, 1.f);
		glColor3f(rgb[0], rgb[1], rgb[2]);
		free(rgb);

		//Lazy, also wrong because Torque swaps y/z
		glVertex3fv(&gTriangles[i].point0);
		glVertex3fv(&gTriangles[i].point1);
		glVertex3fv(&gTriangles[i].point2);
	}
	glEnd();
}

void loop() {
	//Basic movement
	if (movement[0]) gCameraPos[2] += gMovementSpeed;
	if (movement[1]) gCameraPos[2] -= gMovementSpeed;
	if (movement[2]) gCameraPos[0] += gMovementSpeed;
	if (movement[3]) gCameraPos[0] -= gMovementSpeed;
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
		gCameraRot[1] += (GLfloat)((SDL_MouseMotionEvent *)event)->xrel * gCameraSpeed;
		gCameraRot[0] += (GLfloat)((SDL_MouseMotionEvent *)event)->yrel * gCameraSpeed;
	}
}

bool init() {
	//Load our map into triangles (global var warning)
	generateTriangles();

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
	SDL_SetRelativeMouseMode(true);

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

		printf("%f FPS, %f mspf\n", (1000.f / ((double)(end - start) / 1000.0f)), ((double)(end - start) / 1000.0f));
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

	gDifCount = (argc - 1);
	gDifs = malloc(sizeof(DIF *) * gDifCount);
	for (U32 i = 0; i < gDifCount; i ++) {
		//Open file
		FILE *file = fopen(argv[i + 1], "r");

		//Read the .dif
		gDifs[i] = dif_read_file(file);

		//Clean up
		fclose(file);
	}

	//Init SDL and go!
	run();

	//Clean up
	for (U32 i = 0; i < gDifCount; i ++) {
		dif_release(gDifs[i]);
	}
	free(gDifs);

	return 0;
}
