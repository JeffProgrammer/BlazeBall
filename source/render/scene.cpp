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

#include "render/scene.h"
#include "scripting/scriptEngine.h"
#include "game/gameInterior.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>

IMPLEMENT_OBJECT(Scene);

void Scene::loadShaderUniforms() {
	//Light
	glUniform4fv(GLLocations.lightColor, 1, &lightColor.r);
	glUniform4fv(GLLocations.ambientColor, 1, &ambientColor.r);

	//Sun
	glUniform3fv(GLLocations.sunPosition, 1, &sunPosition.x);
	glUniform1f(GLLocations.specularExponent, static_cast<F32>(specularExponent));

	//Projection matrix
	glUniformMatrix4fv(GLLocations.projectionMatrix, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void Scene::render() {
	//Get the camera transform from the marble
	glm::mat4 cameraTransform;
	if (controlObject)
		controlObject->getCameraPosition(cameraTransform);

	//Camera
	viewMatrix = glm::mat4x4(1);
	viewMatrix = glm::rotate(viewMatrix, -90.0f, glm::vec3(1, 0, 0));
	viewMatrix *= cameraTransform;

	//Send to OpenGL
	glUniformMatrix4fv(GLLocations.viewMatrix, 1, GL_FALSE, &viewMatrix[0][0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto object : objects) {
		glm::mat4 modelMatrix(1);
		object->loadMatrix(projectionMatrix, viewMatrix, modelMatrix);
		glUniformMatrix4fv(GLLocations.modelMatrix, 1, GL_FALSE, &modelMatrix[0][0]);
		object->render();
	}
}

void Scene::loop(const F64 &deltaMS) {
	if (controlObject) {
		controlObject->updateCamera(movement, deltaMS);
		controlObject->updateMove(movement, deltaMS);
	}

	movement.pitch = 0;
	movement.yaw = 0;

	if (movement.fire) {
		mEngine->call("fire");
	}

	for (auto object : objects) {
		object->updateTick(fmodf(static_cast<F32>(deltaMS), 16.f));
	}

	mEngine->call("onFrameAdvance", deltaMS);
}

void Scene::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	projectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);
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

	GLLocations.projectionMatrix = shader->getUniformLocation("projectionMat");
	GLLocations.modelMatrix = shader->getUniformLocation("modelMat");
	GLLocations.viewMatrix = shader->getUniformLocation("viewMat");

	GLLocations.lightColor = shader->getUniformLocation("lightColor");
	GLLocations.ambientColor = shader->getUniformLocation("ambientColor");

	GLLocations.sunPosition = shader->getUniformLocation("sunPosition");
	GLLocations.specularExponent = shader->getUniformLocation("specularExponent");

	//Window size for viewport
	glm::ivec2 screenSize = window->getWindowSize();
	updateWindowSize(screenSize);

	loadShaderUniforms();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		// win32 doesn't have gluErrorString
#ifndef _WIN32
		fprintf(stderr, "Error in GL init: %d / %s", err, gluErrorString(err));
#endif
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
				case KeyEvent::KEY_V: window->toggleVsync(); break;
				case KeyEvent::KEY_Q: movement.fire = true; break;
				default:
					mEngine->call("onKeyDown", ((KeyDownEvent *)event)->key);
					break;
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
				case KeyEvent::KEY_Q:     movement.fire      = false; break;
				default:
					mEngine->call("onKeyUp", ((KeyDownEvent *)event)->key);
					break;
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
		case Event::WindowFocus:
			mShouldSleep = false;
			break;
		case Event::WindowBlur:
			mShouldSleep = true;
			break;
		case Event::WindowResize:
			updateWindowSize(static_cast<WindowResizeEvent *>(event)->newSize);
			break;
		default:
			break;
	}
}

bool Scene::init() {
	running = true;
	mShouldSleep = false;

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
	delete mTimer;
	
	//Destroy the SDL
	window->destroyContext();
}

void Scene::run() {
	//Init SDL
	if (!init()) {
		exit(-1);
	}

	Event *eventt;

	F64 lastDelta = 0;
	
	F64 counter = 0;
	U32 fpsCounter = 0;

	//Main loop
	while (running) {
		//Profiling
		mTimer->start();
		
		if (mShouldSleep) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		//Input
		while (window->pollEvents(eventt)) {
			if (eventt != NULL) {
				handleEvent(eventt);
				delete eventt;
			}
		}

		//printf("delta is: %f\n", lastDelta);

		//Hard work
		loop(lastDelta);
		render();
		
		// simulate the physics engine.
		PhysicsEngine::getEngine()->simulate(lastDelta);
		
		//Flip buffers
		window->swapBuffers();

		//Profiling
		if (printFPS) {
			counter += lastDelta;
			fpsCounter++;
			if (counter >= 1000.0) {
				F32 mspf = 1000.0f / fpsCounter;
				std::string title = "FPS: " + std::to_string(fpsCounter) + " mspf: " + std::to_string(mspf);
				window->setWindowTitle(title.c_str());
				
				counter = 0.0;
				fpsCounter = 0;
			}
		}
		
		//Count how long a frame took
		// calculate delta of this elapsed frame.
		mTimer->end();

		lastDelta = mTimer->getDelta();
	}
	
	//Clean up (duh)
	cleanup();
}

Scene::Scene() {
}

Scene::~Scene() {
	for (auto object : objects) {
		delete object;
	}
	delete window;
	delete mTimer;
}

OBJECT_METHOD(Scene, addObject) {
	GameObject *obj = UNWRAP(GameObject, args[0]);
	object->addObject(obj);
}

SCRIPT_FUNCTION(getScene) {
	Isolate *isolate = args.GetIsolate();
	Local<Object> object = ScriptingEngine::instantiateClass(isolate, "Scene");
	Scene *scene = Scene::getSingleton();

	if (scene->persistent().IsEmpty())
		scene->Wrap(object);
	else
		object = scene->handle(isolate);

	args.GetReturnValue().Set(object);
}

OBJECT_METHOD(Scene, setControlObject) {
	GameObject *obj = UNWRAP(GameObject, args[0]);
	object->controlObject = obj;
}

OBJECT_METHOD(Scene, getControlObject) {
	args.GetReturnValue().Set(object->controlObject->handle());
}
