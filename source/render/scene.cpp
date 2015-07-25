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

#include "render/scene.h"
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <libgen.h>
#include <unistd.h>
#include "game/gameInterior.h"

void Scene::render() {
	//Light
	glUniform3fv(lightDirectionLocation, 1, &lightDirection.x);
	glUniform4fv(lightColorLocation, 1, &lightColor.r);
	glUniform4fv(ambientColorLocation, 1, &ambientColor.r);

	glUniform3fv(sunPositionLocation, 1, &sunPosition.x);
	glUniform1f(sunPowerLocation, sunPower);
	glUniform1f(specularExponentLocation, specularExponent);

	//Get the camera transform from the marble
	glm::mat4 cameraTransform;
	sphere->getCameraPosition(cameraTransform);

	//Camera
	viewMatrix = glm::mat4x4(1);
	viewMatrix = glm::rotate(viewMatrix, -90.0f, glm::vec3(1, 0, 0));
	viewMatrix *= cameraTransform;

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

	for (auto interior : interiors) {
		interior->render();
	}

	glm::vec3 pos = sphere->getPosition();
	glm::quat rot = sphere->getRotation();

	//Model
	modelMatrix = glm::mat4x4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, pos.z));
	modelMatrix = glm::rotate(modelMatrix, glm::angle(rot), glm::axis(rot));

	//Combined
	mvpMat = projectionMatrix * viewMatrix * modelMatrix;

	//Send to OpenGL
	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvpMat[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	sphere->render();
}

void Scene::loop() {
	sphere->updateCamera(movement);
	sphere->updateMove(movement);

	movement.pitch = 0;
	movement.yaw = 0;

	if (sphere->getPosition().z < interiors[0]->getInterior().boundingBox.getMin().x) {
		sphere->setPosition(glm::vec3(0, 30, 60));
        sphere->setVelocity(glm::vec3(0, 0, 0));
        sphere->setAngularVelocity(glm::vec3(0, 0, 0));
	}
#if 0
	move *= 3;
	if (movement[8])
		move *= 2;

	delta = glm::rotate(delta, -pitch, glm::vec3(1, 0, 0));
	delta = glm::translate(delta, glm::vec3(move.y, -move.x, 0));

	cameraPosition += glm::vec3(delta[3]);
#endif // 0
}

bool Scene::initGL() {
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	Shader *shader = new Shader("interiorV.glsl", "interiorF.glsl");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	shader->activate();
	shader->setUniformLocation("textureSampler", 0);
	shader->setUniformLocation("normalSampler", 1);
	shader->setUniformLocation("specularSampler", 2);
	shader->setUniformLocation("noiseSampler", 3);

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
	glm::ivec2 screenSize = window->getWindowSize();

	GLfloat aspect = (GLfloat)screenSize.x / (GLfloat)screenSize.y;
	projectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	sphere = new Sphere(glm::vec3(0, 0, 60), 0.2f);
//	sphere->setMaterial(difs[0]->interior[0]->material[4]);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "Error in GL init: %d / %s", err, gluErrorString(err));
		return false;
	}
	return true;
}

void Scene::performClick(S32 mouseX, S32 mouseY) {
	glm::ivec2 screenSize = window->getWindowSize();
	//http://antongerdelan.net/opengl/raycasting.html
	//(x, y) are in device coordinates. We need to convert that to model coords

	//Device coords -> normalized device coords
	float x = (((F32)mouseX * 2.0f) / (F32)screenSize.x) - 1.0f;
	float y = 1.0f - (((F32)mouseY * 2.0f) / (F32)screenSize.y);
	float z = 1.0f;
	glm::vec3 ndc = glm::vec3(x, y, z);

	//Normalized device coords -> clip coordinates
	glm::vec4 clip = glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);

	//Clip coordinates -> eye coordinates
	glm::vec4 eye = glm::inverse(projectionMatrix) * clip;
	eye = glm::vec4(eye.x, eye.y, -1.0f, 0.0f);

	//Eye coordinates -> modelview coordinates
	glm::vec3 world = glm::vec3(glm::inverse(viewMatrix) * eye);

//	RayF ray(cameraPosition.x, cameraPosition.y, cameraPosition.z,
//			 world.x, world.y, world.z);

//	for (U32 i = 0; i < difCount; i ++) {
//		DIF *dif = difs[i];
//		for (U32 j = 0; j < dif->numDetailLevels; j ++) {
//			Interior *interior = dif->interior[j];
//
//			U32 surfaceNum = interior->rayCast(ray);
//			if (surfaceNum != -1) {
//				selection.hasSelection = true;
//				selection.surfaceIndex = surfaceNum;
//				selection.interior = interior;
//				return;
//			}
//		}
//	}

	selection.hasSelection = false;
}

void Scene::handleEvent(Event *event) {
	//Key events, movement
	switch (event->getType()) {
		//Quit
		case Event::Quit:
			running = false;
			break;
		case Event::KeyDown:
			switch (((KeyDownEvent *)event)->key) {
					//Same for Colemak...
				case KeyEvent::KEY_W:     movement.forward   = true; break;
				case KeyEvent::KEY_S:     movement.backward  = true; break;
				case KeyEvent::KEY_A:     movement.left      = true; break;
				case KeyEvent::KEY_D:     movement.right     = true; break;
				case KeyEvent::KEY_UP:    movement.pitchUp   = true; break;
				case KeyEvent::KEY_DOWN:  movement.pitchDown = true; break;
				case KeyEvent::KEY_LEFT:  movement.yawLeft   = true; break;
				case KeyEvent::KEY_RIGHT: movement.yawRight  = true; break;
				case KeyEvent::KEY_SPACE: movement.jump      = true; break;
				default: break;
			}
			break;
		case Event::KeyUp:
			switch (((KeyUpEvent *)event)->key) {
				case KeyEvent::KEY_W:     movement.forward   = false; break;
				case KeyEvent::KEY_S:     movement.backward  = false; break;
				case KeyEvent::KEY_A:     movement.left      = false; break;
				case KeyEvent::KEY_D:     movement.right     = false; break;
				case KeyEvent::KEY_UP:    movement.pitchUp   = false; break;
				case KeyEvent::KEY_DOWN:  movement.pitchDown = false; break;
				case KeyEvent::KEY_LEFT:  movement.yawLeft   = false; break;
				case KeyEvent::KEY_RIGHT: movement.yawRight  = false; break;
				case KeyEvent::KEY_SPACE: movement.jump      = false; break;
				default: break;
			}
			break;
		//Mouse for rotation
		case Event::MouseMove:
			if (mouseButtons.right) {
				movement.yaw += (GLfloat)((MouseMoveEvent *)event)->delta.x;
				movement.pitch += (GLfloat)((MouseMoveEvent *)event)->delta.y;
			}
			break;
		case Event::MouseDown:
			switch (((MouseDownEvent *)event)->button) {
				case 1: mouseButtons.left   = true; break;
				case 2: mouseButtons.middle = true; break;
				case 3: mouseButtons.right  = true; break;
				default: break;
			}

			if (((MouseDownEvent *)event)->button == 3) { //Right mouse: lock cursor
				window->lockCursor(true);
			}

			if (((MouseDownEvent *)event)->button == 1) { //Left mouse: click
				performClick(((MouseDownEvent *)event)->position.x, ((MouseDownEvent *)event)->position.y);
			}
			break;
		case Event::MouseUp:
			switch (((MouseDownEvent *)event)->button) {
				case 1: mouseButtons.left   = false; break;
				case 2: mouseButtons.middle = false; break;
				case 3: mouseButtons.right  = false; break;
				default: break;
			}

			if (((MouseUpEvent *)event)->button == 3) { //Right mouse: lock cursor
				window->lockCursor(false);
			}
			break;
		default:
			break;
	}
}

bool Scene::init() {
	running = true;

	if (!window->createContext()) {
		return false;
	}

	//Initialize OpenGL
	if (!initGL()) {
		return false;
	}

	captureMouse = true;

	return true;
}

void Scene::cleanup() {
	//Destroy the SDL
	window->destroyContext();
}

void Scene::run() {
	//Init SDL
	if (!init()) {
		exit(-1);
	}

	Event *event;

	//Main loop
	while (running) {
		//Profiling
		struct timeval startTime, endTime;
		gettimeofday(&startTime, NULL);

		//Input
		while (window->pollEvents(&event)) {
			if (event != NULL) {
				handleEvent(event);
				delete event;
			}
		}

		//Hard work
		loop();
		if (loopCallback)
			loopCallback();
		render();

		//Flip buffers
		window->swapBuffers();

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

		PhysicsEngine::getEngine()->simulate((end - start) / 1000000.0f);
	}
	
	//Clean up (duh)
	cleanup();
}
