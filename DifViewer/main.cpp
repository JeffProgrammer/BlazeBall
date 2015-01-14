//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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
#include <unistd.h>
#include "math.h"
#include "types.h"
#include "io.h"
#include "dif.h"
#include "texture.h"
#include "sphere.h"
#include "physics.h"
#include "shader.h"

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

//TRIGGER WARNING: LOTS OF GLOBAL VARIABLES. I BET "NOBODY" WOULD LOVE THIS.

U32 gDifCount;
DIF **gDifs;

Sphere *gSphere;

String **gFilenames;

bool gRunning;
bool gPrintFPS = false;

F32 gMaxFPS = 60.0f;

SDL_Window *gWindow;
SDL_GLContext gContext;
GLfloat gAngle;

float gYaw, gPitch;
glm::vec3 gCameraPosition;

GLuint mvpMatrix, lightDirection;

glm::mat4x4 gProjectionMatrix, gModelviewMatrix;

struct {
	bool hasSelection;
	U32 surfaceIndex;
	Interior *interior;
} gSelection;

static float gCameraSpeed = 0.3f;
static float gKeyCameraSpeed = 3.f;
static float gMovementSpeed = 0.2f;

static F32 gLightColor[4]     = {1.100000f, 1.100000f, 0.900000f, 1.000000f};
static F32 gLightDirection[4] = {0.60f, 0.40f, 1.0f, 0.0f};
static F32 gAmbientColor[4]   = {0.600000f, 0.600000f, 0.800000f, 1.000000f};
static F32 gDiffuseColor[4]   = {0.800000f, 0.800000f, 1.000000f, 1.000000f};

bool captureMouse = false;
bool mouseButtons[3] = {false, false, false};
bool movement[9] = {false, false, false, false, false, false, false, false, false};

GLuint gDisplayList = 0;
bool gListNeedsDisplay = false;

void render();
void loop();
bool initGL();
bool init();
void cleanup();
void run();

void performClick(S32 mouseX, S32 mouseY);
void handleEvent(SDL_Event *event);
const char *openFile();

void render() {
#ifdef GL_33
	//Camera
	gModelviewMatrix = glm::mat4x4(1);
	gModelviewMatrix = glm::rotate(gModelviewMatrix, gPitch, glm::vec3(1, 0, 0));
	gModelviewMatrix = glm::rotate(gModelviewMatrix, gYaw, glm::vec3(0, 1, 0));
	gModelviewMatrix = glm::rotate(gModelviewMatrix, -90.0f, glm::vec3(1, 0, 0));
	gModelviewMatrix = glm::translate(gModelviewMatrix, -gCameraPosition);

	glm::mat4x4 mvpMat = gProjectionMatrix * gModelviewMatrix;

	glUniformMatrix4fv(mvpMatrix, 1, GL_FALSE, &mvpMat[0][0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3fv(lightDirection, 1, gLightDirection);

	for (U32 index = 0; index < gDifCount; index ++) {
		gDifs[index]->render();
	}

	btTransform trans;
	Point3F pos = gSphere->getPosition();
	AngAxisF rot = gSphere->getRotation();

	glm::mat4x4 mat = glm::mat4x4(1);
	mat = glm::translate(mat, glm::vec3(pos.x, pos.y, pos.z));
	mat = glm::rotate(mat, rot.angle * (F32)(180.0f / M_PI), glm::vec3(rot.axis.x, rot.axis.y, rot.axis.z));

	mvpMat *= mat;
	glUniformMatrix4fv(mvpMatrix, 1, GL_FALSE, &mvpMat[0][0]);

	gSphere->render(ColorF(1, 1, 0, 1));
#else
	//Load the model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Camera
	gModelviewMatrix = glm::mat4x4(1);
	gModelviewMatrix = glm::rotate(gModelviewMatrix, gPitch, glm::vec3(1, 0, 0));
	gModelviewMatrix = glm::rotate(gModelviewMatrix, gYaw, glm::vec3(0, 1, 0));
	gModelviewMatrix = glm::rotate(gModelviewMatrix, -90.0f, glm::vec3(1, 0, 0));
//	gModelviewMatrix = glm::translate(gModelviewMatrix, -gCameraPosition);
	gModelviewMatrix = glm::translate(gModelviewMatrix, gCameraPosition);
	
	glLoadMatrixf(&gModelviewMatrix[0][0]);

	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, gLightDirection);

	Point3F offset = {0.f, 0.f, 0.f};

	if (gListNeedsDisplay) {
		gDisplayList = glGenLists(1);
		glNewList(gDisplayList, GL_COMPILE_AND_EXECUTE);
		for (U32 index = 0; index < gDifCount; index ++) {
			gDifs[index]->render();
		}
		glEndList();
		gListNeedsDisplay = false;
	} else {
		glCallList(gDisplayList);
	}

	gSphere->render(ColorF(1, 1, 0, 1));
	glDisable(GL_CULL_FACE);

	if (gSelection.hasSelection) {
		Surface surface = gSelection.interior->surface[gSelection.surfaceIndex];
		TexGenEq texGenEq = gSelection.interior->texGenEq[surface.texGenIndex];
		Point3F normal = gSelection.interior->normal[gSelection.interior->plane[surface.planeIndex].normalIndex];
		Point3F first = gSelection.interior->point[gSelection.interior->index[surface.windingStart]];
		F32 len = 0;

		glBegin(GL_TRIANGLE_STRIP);
		for (U32 i = 0; i < surface.windingCount; i ++) {
			Point3F vert = gSelection.interior->point[gSelection.interior->index[surface.windingStart + i]];

			glVertex3f(vert.x, vert.y, vert.z);

			Point2F point = point3F_project_plane(vert, normal, first);
			F32 distance = point.length();
			len = (distance > len ? distance : len);
		}
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, -100, 100);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glLoadIdentity();

		Texture *texture = gSelection.interior->texture[surface.textureIndex];
		if (texture) {
			if (!texture->generated) {
				texture->generateBuffer();
			}
			texture->activate();
		}

		glBegin(GL_TRIANGLE_STRIP);

		int w, h;
		SDL_GetWindowSize(gWindow, &w, &h);

		GLfloat aspect = (GLfloat)w / (GLfloat)h;

		for (U32 i = 0; i < surface.windingCount; i ++) {
			Point3F vert = gSelection.interior->point[gSelection.interior->index[surface.windingStart + i]];

			Point2F texuv = Point2F(planeF_distance_to_point(texGenEq.planeX, vert), planeF_distance_to_point(texGenEq.planeY, vert));
			glTexCoord2fv(&texuv.x);

			Point2F point = point3F_project_plane(vert, (surface.planeFlipped ? normal * -1 : normal), first);
			glVertex3f(point.x / len / aspect, point.y / len, 0);
		}
		glEnd();

		if (texture)
			texture->deactivate();
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
#endif
}

void loop() {
	//Basic movement
	glm::mat4x4 delta = glm::mat4x4(1);

	if (movement[4]) gPitch -= gKeyCameraSpeed;
	if (movement[5]) gPitch += gKeyCameraSpeed;
	if (movement[6]) gYaw -= gKeyCameraSpeed;
	if (movement[7]) gYaw += gKeyCameraSpeed;

	delta = glm::rotate(delta, -gYaw, glm::vec3(0, 0, 1));

	float speed = gMovementSpeed;
	if (mouseButtons[1])
		speed *= 2.f;

	glm::vec3 torque;
	Point2F move = Point2F();
	if (movement[0]) move.x -= speed;
	if (movement[1]) move.x += speed;
	if (movement[2]) move.y -= speed;
	if (movement[3]) move.y += speed;
	torque = glm::vec3(glm::translate(delta, glm::vec3(move.x, move.y, 0))[3]);
	delta = glm::rotate(delta, -gPitch, glm::vec3(1, 0, 0));

	torque /= 3.0;

	Point3F force = Point3F(torque.x, torque.y, torque.z);
	gSphere->applyTorque(force);

	if (gSphere->colliding()) {
		Point3F normal = gSphere->getCollisionNormal();
		if (movement[8] && normal.dot(Point3F(0, 0, 1)) > 0.1)
			gSphere->applyImpulse((normal + Point3F(0, 0, 1)) / 2.f * 7.5f, Point3F(0, 0, -1));
	} else {
		gSphere->applyImpulse(Point3F(torque.y, -torque.x, torque.z), Point3F(0, 0, 0));
	}

	Point3F pos = gSphere->getPosition();
	gCameraPosition = glm::vec3(pos.x, pos.y, pos.z);
	gCameraPosition += glm::vec3(glm::translate(delta, glm::vec3(0, -2.5, 0))[3]);

	if (gSphere->getPosition().z < gDifs[0]->interior[0]->boundingBox.getMin().x) {
		gSphere->setPosition(Point3F(0, 30, 60));
	}
}

bool initGL() {
#ifdef GL_33
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	Shader *shader = new Shader(String("vertexShader.glsl"), String("fragmentShader.glsl"));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	shader->activate();
	shader->setUniformLocation(String("noiseSampler"), 0);
	shader->setUniformLocation(String("textureSampler"), 1);

	mvpMatrix = glGetUniformLocation(shader->getProgramId(), "MVP");
	lightDirection = glGetUniformLocation(shader->getProgramId(), "lightDirection");

	//Window size for viewport
	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);

	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	gProjectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	gSphere = new Sphere(Point3F(0, 30, 60), 0.2f);
	String directory = String(dirname((char *)gFilenames[0]));
	directory.concat("/");
	directory.concat("tile_beginner.png");
	gSphere->setTexture(directory);
#else
	//Initialize clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClearDepth(1.0f);

	//Enable and set some shit for rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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

	glMaterialfv(GL_FRONT, GL_AMBIENT, gAmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gDiffuseColor);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, gLightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, gLightDirection);
#endif

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "Error in GL init: %d / %s", err, gluErrorString(err));
		return false;
	}
	return true;
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

	RayF ray(gCameraPosition.x, gCameraPosition.y, gCameraPosition.z,
			 world.x, world.y, world.z);

	for (U32 i = 0; i < gDifCount; i ++) {
		DIF *dif = gDifs[i];
		for (U32 j = 0; j < dif->numDetailLevels; j ++) {
			Interior *interior = dif->interior[j];

			U32 surfaceNum = interior->rayCast(ray);
			if (surfaceNum != -1) {
				gSelection.hasSelection = true;
				gSelection.surfaceIndex = surfaceNum;
				gSelection.interior = interior;
				return;
			}
		}
	}

	gSelection.hasSelection = false;
}

void handleEvent(SDL_Event *event) {
	//Quit
	if (event->type == SDL_QUIT) {
		gRunning = false;
	}

	//Key events, movement
	if (event->type == SDL_KEYDOWN) {
		switch (((SDL_KeyboardEvent *)event)->keysym.scancode) {
			//Same for Colemak...
			case SDL_SCANCODE_W: movement[0] = true; break;
			case SDL_SCANCODE_S:
				if (((SDL_KeyboardEvent *)event)->keysym.mod & KMOD_LGUI) { //LGUI -> LCmd
					//Save
					for (U32 i = 0; i < gDifCount; i ++) {
						String directory = String(dirname((char *)gFilenames[i]));

						FILE *output = fopen((const char *)gFilenames[i], "w");
						gDifs[i]->write(output, directory);
						fflush(output);
						fclose(output);
					}
				} else {
					movement[1] = true;
				}
				break;
			case SDL_SCANCODE_A: movement[2] = true; break;
			case SDL_SCANCODE_D: movement[3] = true; break;
			case SDL_SCANCODE_UP:    movement[4] = true; break;
			case SDL_SCANCODE_DOWN:  movement[5] = true; break;
			case SDL_SCANCODE_LEFT:  movement[6] = true; break;
			case SDL_SCANCODE_RIGHT: movement[7] = true; break;
			case SDL_SCANCODE_SPACE: movement[8] = true; break;
			default: break;
		}
	} else if (event->type == SDL_KEYUP) {
		switch (((SDL_KeyboardEvent *)event)->keysym.scancode) {
			case SDL_SCANCODE_W: movement[0] = false; break;
			case SDL_SCANCODE_S:
				movement[1] = false;
				break;
			case SDL_SCANCODE_A: movement[2] = false; break;
			case SDL_SCANCODE_D: movement[3] = false; break;
			case SDL_SCANCODE_UP:    movement[4] = false; break;
			case SDL_SCANCODE_DOWN:  movement[5] = false; break;
			case SDL_SCANCODE_LEFT:  movement[6] = false; break;
			case SDL_SCANCODE_RIGHT: movement[7] = false; break;
			case SDL_SCANCODE_Q: gListNeedsDisplay = true; break;
			case SDL_SCANCODE_SPACE: movement[8] = false; break;
			default: break;
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
	gListNeedsDisplay = true;

	//Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

#ifdef GL_33
	//Use OpenGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#else
	//Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 4);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

	//Initialize OpenGL
	if (!initGL()) {
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		return false;
	}

	//Lock cursor
	SDL_SetRelativeMouseMode(SDL_TRUE);
	captureMouse = true;

//	Point3F center = gDifs[0]->interior[0]->boundingBox.getCenter();
	gCameraPosition = glm::vec3(0, 0, 0);

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

		//Count how long a frame took
		gettimeofday(&endTime, NULL);
		long long start = startTime.tv_usec + (startTime.tv_sec * 1000000ULL);
		long long end = endTime.tv_usec + (endTime.tv_sec * 1000000ULL);

		//If our app is in the background, OS X just ignores all render calls and immediately pushes through.
		//This is bad. This means that we get ~4000 "FPS" because no frames are actually drawn.
		//This code here mitigates that by sleeping away the extra frames that OS X skips.
		if (end - start < (1000000.0f / gMaxFPS)) {
			usleep((1000000.0f / gMaxFPS) - (end - start));
		}

		//Profiling
		gettimeofday(&endTime, NULL);
		end = endTime.tv_usec + (endTime.tv_sec * 1000000ULL);
		if (gPrintFPS) {
			printf("%f FPS, %f mspf\n", (1000.f / ((double)(end - start) / 1000.0f)), ((double)(end - start) / 1000.0f));
		}

		Physics::getPhysics()->simulate((end - start) / 1000000.0f);
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

	Physics::getPhysics()->init();

	U32 argstart = 1;

	if (!strcmp(argv[1], "-o")) {
		argstart += 2;
	}
	if (!strcmp(argv[1], "-c")) {
		argstart += 1;
	}

	gDifCount = 0;
	gDifs = new DIF*[argc - argstart];
	gFilenames = new String*[argc - argstart];

	for (U32 i = 0; i < (argc - argstart); i ++) {
		String directory = String(dirname((char *)argv[i + argstart]));

		//Open file
		FILE *file = fopen(argv[i + argstart], "r");

		//Read the .dif
		gDifs[i] = new DIF(file, directory);
		if (gDifs[i]) {
			gDifCount ++;
		}

		//Clean up
		fclose(file);

		gFilenames[i] = new String(argv[i + argstart]);
	}

	if (!strcmp(argv[1], "-o")) {
		FILE *out = fopen(argv[2], "w");
		gDifs[0]->interior[0]->exportObj(out);
		fflush(out);
		fclose(out);
	} else if (!strcmp(argv[1], "-c")) {
		for (U32 i = 0; i < gDifCount; i ++) {
			String directory = String(dirname((char *)gFilenames[i]));

			FILE *output = fopen((const char *)gFilenames[i], "w");
			gDifs[i]->write(output, directory);
			fflush(output);
			fclose(output);
		}
	} else {
		//Init SDL and go!
		run();
	}

	//Clean up
	delete [] gDifs;
	delete [] gFilenames;

	return 0;
}
