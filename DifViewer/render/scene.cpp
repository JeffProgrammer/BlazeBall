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

#include "scene.h"
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <libgen.h>
#include <unistd.h>

void Scene::render() {
#ifdef GL_33
	//Light
	glUniform3fv(lightDirectionLocation, 1, &lightDirection.x);
	glUniform4fv(lightColorLocation, 1, &lightColor.red);
	glUniform4fv(ambientColorLocation, 1, &ambientColor.red);

	glUniform3fv(sunPositionLocation, 1, &sunPosition.x);
	glUniform1f(sunPowerLocation, sunPower);
	glUniform1f(specularExponentLocation, specularExponent);

	//Camera
	viewMatrix = glm::mat4x4(1);
	viewMatrix = glm::rotate(viewMatrix, pitch, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, yaw, glm::vec3(0, 1, 0));
	viewMatrix = glm::rotate(viewMatrix, -90.0f, glm::vec3(1, 0, 0));
	viewMatrix = glm::translate(viewMatrix, -cameraPosition);

	//Model
	modelMatrix = glm::mat4x4(1);

	//Combined
	glm::mat4x4 mvpMat = projectionMatrix * viewMatrix * modelMatrix;
	glm::mat3x3 mv3Mat = glm::mat3(viewMatrix * modelMatrix);

	//Send to OpenGL
	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvpMat[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix3fv(modelView3Location, 1, GL_FALSE, &mv3Mat[0][0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (U32 index = 0; index < difCount; index ++) {
		difs[index]->render();
	}

#ifdef BUILD_PHYSICS
	btTransform trans;
	Point3F pos = sphere->getPosition();
	AngAxisF rot = sphere->getRotation();

	//Model
	modelMatrix = glm::mat4x4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, pos.z));
	modelMatrix = glm::rotate(modelMatrix, rot.angle * (F32)(180.0f / M_PI), glm::vec3(rot.axis.x, rot.axis.y, rot.axis.z));

	//Combined
	mvpMat = projectionMatrix * viewMatrix * modelMatrix;

	//Send to OpenGL
	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvpMat[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	sphere->render(ColorF(1, 1, 0, 1));
#endif
#else
	//Load the model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Camera
	viewMatrix = glm::mat4x4(1);
	viewMatrix = glm::rotate(viewMatrix, pitch, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, yaw, glm::vec3(0, 1, 0));
	viewMatrix = glm::rotate(viewMatrix, -90.0f, glm::vec3(1, 0, 0));
	//	viewMatrix = glm::translate(viewMatrix, -cameraPosition);
	viewMatrix = glm::translate(viewMatrix, cameraPosition);

	glLoadMatrixf(&modelviewMatrix[0][0]);

	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, gLightDirection);

	Point3F offset = {0.f, 0.f, 0.f};

	if (listNeedsDisplay) {
		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE_AND_EXECUTE);
		for (U32 index = 0; index < difCount; index ++) {
			difs[index]->render();
		}
		glEndList();
		listNeedsDisplay = false;
	} else {
		glCallList(displayList);
	}

	sphere->render(ColorF(1, 1, 0, 1));
	glDisable(GL_CULL_FACE);

	if (selection.hasSelection) {
		Surface surface = selection.interior->surface[selection.surfaceIndex];
		TexGenEq texGenEq = selection.interior->texGenEq[surface.texGenIndex];
		Point3F normal = selection.interior->normal[selection.interior->plane[surface.planeIndex].normalIndex];
		Point3F first = selection.interior->point[selection.interior->index[surface.windingStart]];
		F32 len = 0;

		glBegin(GL_TRIANGLE_STRIP);
		for (U32 i = 0; i < surface.windingCount; i ++) {
			Point3F vert = selection.interior->point[selection.interior->index[surface.windingStart + i]];

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

		Texture *texture = selection.interior->texture[surface.textureIndex];
		if (texture) {
			if (!texture->generated) {
				texture->generateBuffer();
			}
			texture->activate();
		}

		glBegin(GL_TRIANGLE_STRIP);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);

		GLfloat aspect = (GLfloat)w / (GLfloat)h;

		for (U32 i = 0; i < surface.windingCount; i ++) {
			Point3F vert = selection.interior->point[selection.interior->index[surface.windingStart + i]];

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

void Scene::loop() {
	//Basic movement
	glm::mat4x4 delta = glm::mat4x4(1);

	if (movement[4]) pitch -= keyCameraSpeed;
	if (movement[5]) pitch += keyCameraSpeed;
	if (movement[6]) yaw -= keyCameraSpeed;
	if (movement[7]) yaw += keyCameraSpeed;

	delta = glm::rotate(delta, -yaw, glm::vec3(0, 0, 1));

	float speed = movementSpeed;
	if (mouseButtons[1])
		speed *= 2.f;

	glm::vec3 torque;
	Point2F move = Point2F();
	if (movement[0]) move.x -= speed;
	if (movement[1]) move.x += speed;
	if (movement[2]) move.y -= speed;
	if (movement[3]) move.y += speed;
	torque = glm::vec3(glm::translate(delta, glm::vec3(move.x, move.y, 0))[3]);
	delta = glm::rotate(delta, -pitch, glm::vec3(1, 0, 0));

	torque /= 3.0;

	Point3F force = Point3F(torque.x, torque.y, torque.z);
	sphere->applyTorque(force);

	if (sphere->colliding()) {
		Point3F normal = sphere->getCollisionNormal();
		if (movement[8] && normal.dot(Point3F(0, 0, 1)) > 0.1)
			sphere->applyImpulse((normal + Point3F(0, 0, 1)) / 2.f * 5.f, Point3F(0, 0, -1));
	} else {
		sphere->applyImpulse(Point3F(torque.y, -torque.x, torque.z), Point3F(0, 0, 0));
	}

	Point3F pos = sphere->getPosition();
	cameraPosition = glm::vec3(pos.x, pos.y, pos.z);
	cameraPosition += glm::vec3(glm::translate(delta, glm::vec3(0, -2.5, 0))[3]);

	if (sphere->getPosition().z < difs[0]->interior[0]->boundingBox.getMin().x) {
		sphere->setPosition(Point3F(0, 30, 60));
	}
}

bool Scene::initGL() {
#ifdef GL_33
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	Shader *shader = new Shader(String("interiorV.glsl"), String("interiorF.glsl"));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	shader->activate();
	shader->setUniformLocation(String("textureSampler"), 0);
	shader->setUniformLocation(String("normalSampler"), 1);
	shader->setUniformLocation(String("specularSampler"), 2);
	shader->setUniformLocation(String("noiseSampler"), 3);

	mvpMatrixLocation = shader->getUniformLocation("modelViewProjectionMat");
	modelMatrixLocation = shader->getUniformLocation("modelMat");
	viewMatrixLocation = shader->getUniformLocation("viewMat");
	modelView3Location = shader->getUniformLocation("modelView3Mat");

	lightDirectionLocation = shader->getUniformLocation("lightDirection");
	lightColorLocation = shader->getUniformLocation("lightColor");
	ambientColorLocation = shader->getUniformLocation("ambientColor");

	sunPositionLocation = shader->getUniformLocation("sunPosition");
	sunPowerLocation = shader->getUniformLocation("sunPower");
	specularExponentLocation = shader->getUniformLocation("specularExponent");

	//Window size for viewport
	Point2I screenSize = window->getWindowSize();

	GLfloat aspect = (GLfloat)screenSize.x / (GLfloat)screenSize.y;
	projectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	sphere = new Sphere(Point3F(0, 0, 60), 0.2f);
	sphere->setMaterial(difs[0]->interior[0]->material[4]);
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
	SDL_GetWindowSize(window, &w, &h);

	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w * 2, h * 2);

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);
	glMultMatrixf(&projectionMatrix[0][0]);

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

void Scene::performClick(S32 mouseX, S32 mouseY) {
	int screenWidth, screenHeight;
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

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
	glm::vec4 eye = glm::inverse(projectionMatrix) * clip;
	eye = glm::vec4(eye.x, eye.y, -1.0f, 0.0f);

	//Eye coordinates -> modelview coordinates
	glm::vec3 world = glm::vec3(glm::inverse(viewMatrix) * eye);

	RayF ray(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			 world.x, world.y, world.z);

	for (U32 i = 0; i < difCount; i ++) {
		DIF *dif = difs[i];
		for (U32 j = 0; j < dif->numDetailLevels; j ++) {
			Interior *interior = dif->interior[j];

			U32 surfaceNum = interior->rayCast(ray);
			if (surfaceNum != -1) {
				selection.hasSelection = true;
				selection.surfaceIndex = surfaceNum;
				selection.interior = interior;
				return;
			}
		}
	}

	selection.hasSelection = false;
}

void Scene::handleEvent(SDL_Event *event) {
	//Quit
	if (event->type == SDL_QUIT) {
		running = false;
	}

	//Key events, movement
	if (event->type == SDL_KEYDOWN) {
		switch (((SDL_KeyboardEvent *)event)->keysym.scancode) {
				//Same for Colemak...
			case SDL_SCANCODE_W: movement[0] = true; break;
			case SDL_SCANCODE_S:
				if (((SDL_KeyboardEvent *)event)->keysym.mod & KMOD_LGUI) { //LGUI -> LCmd
					//Save
					for (U32 i = 0; i < difCount; i ++) {
						String directory = io->getPath(*filenames[i]);

						FILE *output = fopen(*filenames[i], "w");
						difs[i]->write(output, directory);
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
			case SDL_SCANCODE_Q: listNeedsDisplay = true; break;
			case SDL_SCANCODE_SPACE: movement[8] = false; break;
			default: break;
		}
	}
	//Mouse for rotation
	if (event->type == SDL_MOUSEMOTION) {
		if (mouseButtons[2]) {
			yaw += (GLfloat)((SDL_MouseMotionEvent *)event)->xrel * cameraSpeed;
			pitch += (GLfloat)((SDL_MouseMotionEvent *)event)->yrel * cameraSpeed;
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

bool Scene::init() {
	running = true;
	listNeedsDisplay = true;

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
	if ((window = SDL_CreateWindow("DIF Viewer", (bounds.w - 1280) / 2, (bounds.h - 720) / 2, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)) == NULL) {
		return false;
	}

	//Create context
	if ((context = SDL_GL_CreateContext(window)) == NULL) {
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

	//	Point3F center = difs[0]->interior[0]->boundingBox.getCenter();
	cameraPosition = glm::vec3(0, 0, 0);

	return true;
}

void Scene::cleanup() {
	//Destroy the SDL
	SDL_Quit();
}

void Scene::run() {
	//Init SDL
	if (!init()) {
		exit(-1);
	}

	SDL_Event event;

	//Main loop
	while (running) {
		//Profiling
		struct timeval startTime, endTime;
		gettimeofday(&startTime, NULL);

		//Input
		while (SDL_PollEvent(&event)) {
			handleEvent(&event);
		}

		//Hard work
		loop();
		if (loopCallback)
			loopCallback();
		render();

		//Flip buffers
		SDL_GL_SwapWindow(window);

		//Count how long a frame took
		gettimeofday(&endTime, NULL);
		long long start = startTime.tv_usec + (startTime.tv_sec * 1000000ULL);
		long long end = endTime.tv_usec + (endTime.tv_sec * 1000000ULL);

		//If our app is in the background, OS X just ignores all render calls and immediately pushes through.
		//This is bad. This means that we get ~4000 "FPS" because no frames are actually drawn.
		//This code here mitigates that by sleeping away the extra frames that OS X skips.
		if (end - start < (1000000.0f / maxFPS)) {
			usleep((1000000.0f / maxFPS) - (end - start));
		}

		//Profiling
		gettimeofday(&endTime, NULL);
		end = endTime.tv_usec + (endTime.tv_sec * 1000000ULL);
		if (printFPS) {
			printf("%f FPS, %f mspf\n", (1000.f / ((double)(end - start) / 1000.0f)), ((double)(end - start) / 1000.0f));
		}
		
		Physics::getPhysics()->simulate((end - start) / 1000000.0f);
	}
	
	//Clean up (duh)
	cleanup();
}
