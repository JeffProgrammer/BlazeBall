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
#include "game/gameInterior.h"
#include "game/camera.h"
#include "game/Shape.h"
#include "game/skybox.h"
#include "render/util.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>

glm::mat4 RenderInfo::inverseRotMat = glm::rotate(glm::mat4x4(1), -90.0f, glm::vec3(1, 0, 0));

/// The amount of time that has to pass before a tick happens.
/// Default is 16.6667 ms which means we tick at 60 frames per second
#define TICK_MS 16.6666666666666667

Scene::Scene() {
	mInteriorShader = nullptr;
	mShapeShader = nullptr;
	controlObject = nullptr;
}

Scene::~Scene() {
	for (auto object : objects) {
		delete object;
	}
	delete window;
	delete mTimer;
}

void Scene::render() {
	//Get the camera transform from the marble
	glm::mat4 cameraTransform;
	glm::vec3 cameraPosition;
	if (controlObject)
		controlObject->getCameraPosition(cameraTransform, cameraPosition);

	//todo: make the lambda take renderinfo
	marbleCubemap->generateBuffer(mPlayer->getPosition(), window->getWindowSize() * (S32)pixelDensity, [&](const glm::mat4 &projectionMat, const glm::mat4 &viewMat) {
		RenderInfo info;
		info.projectionMatrix = projectionMat;
		info.viewMatrix = viewMat;
		info.cameraPosition = mPlayer->getPosition();
		
		info.lightColor = lightColor;
		info.ambientColor = ambientColor;
		info.sunPosition = sunPosition;
		info.specularExponent = specularExponent;

		info.isReflectionPass = true;

		this->renderScene(info);
	});

	//Camera
	glm::mat4 viewMatrix = glm::mat4x4(1);
	viewMatrix = glm::rotate(viewMatrix, -90.0f, glm::vec3(1, 0, 0));
	viewMatrix *= cameraTransform;

	RenderInfo info;
	info.projectionMatrix = mScreenProjectionMatrix;
	info.viewMatrix = viewMatrix;
	info.cameraPosition = cameraPosition;

	info.lightColor = lightColor;
	info.ambientColor = ambientColor;
	info.sunPosition = sunPosition;
	info.specularExponent = specularExponent;

	info.isReflectionPass = false;

	//Actually render everything
	renderScene(info);
}

void Scene::renderScene(const RenderInfo &info) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Send to OpenGL
	for (auto object : objects) {
		if (object->isRenderable() && (!info.isReflectionPass || object->isReflectable())) {
			dynamic_cast<IRenderedObject *>(object)->render(info);
		}
	}

	// render models.
	// yeah i know this is shitty rendering at the moment, we need to actually batch shit.
	mShapeShader->activate();
	info.loadShader(mShapeShader);

	MODELMGR->render(mShapeShader, info.viewMatrix, info.projectionMatrix);
	mShapeShader->deactivate();

	// check for opengl errors
	GL_CHECKERRORS();
}

void Scene::loop(const F64 &deltaMS) {

}

void Scene::tick(const F64 &deltaMS) {
	if (controlObject) {
		controlObject->updateCamera(movement, deltaMS);
		controlObject->updateMove(movement, deltaMS);
	}

	movement.pitch = 0;
	movement.yaw = 0;

	if (movement.fire) {

	}

	for (auto object : objects) {
		object->updateTick(deltaMS);
	}
}

void Scene::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	mScreenProjectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);
}

bool Scene::initGL() {
	Shader::defaultShader = new Shader("defaultV.glsl", "defaultF.glsl");

	mInteriorShader = new Shader("interiorV.glsl", "interiorF.glsl");
	mShapeShader = new Shader("modelV.glsl", "modelF.glsl");
	mSkyboxShader = new Shader("skyboxV.glsl", "skyboxF.glsl");

	mInteriorShader->addUniformLocation("textureSampler", 0);
	mInteriorShader->addUniformLocation("normalSampler", 1);
	mInteriorShader->addUniformLocation("specularSampler", 2);
	mInteriorShader->addUniformLocation("noiseSampler", 3);
	mInteriorShader->addUniformLocation("skyboxSampler", 4);
	mInteriorShader->addUniformLocation("cubemapSampler", 5);

	mInteriorShader->addAttribute("vertexPosition",  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	mInteriorShader->addAttribute("vertexUV",        2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	mInteriorShader->addAttribute("vertexNormal",    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	mInteriorShader->addAttribute("vertexTangent",   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	mInteriorShader->addAttribute("vertexBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	mShapeShader->addUniformLocation("textureSampler", 0);
	mShapeShader->addUniformLocation("normalSampler", 1);
	mShapeShader->addUniformLocation("specularSampler", 2);

	mSkyboxShader->addUniformLocation("cubemapSampler", 0);
	mSkyboxShader->addAttribute("vertexPosition", 3, GL_FLOAT, GL_FALSE, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	//Window size for viewport
	glm::ivec2 screenSize = window->getWindowSize();
	updateWindowSize(screenSize);

	GLint viewport[4]; //x y w h
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Should be 2x if you have a retina display
	pixelDensity = viewport[2] / screenSize.x;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	marbleCubemap = new CubeMapFramebufferTexture(glm::ivec2(64));
	marbleCubemap->generateBuffer();

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
	/*
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
	*/
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
				case KeyEvent::KEY_Q:
				{
					movement.fire = true;

					// make a new sphere!
					Sphere *sphere = new Sphere(glm::vec3(0, 0, 30), 0.75);
					addObject(sphere);

					break;
				}
				case KeyEvent::KEY_C:
				{
					// swap control objects!
					if (controlObject == mPlayer)
						controlObject = mCamera;
					else
						controlObject = mPlayer;
					break;
				}
				case KeyEvent::KEY_M:
				{
					// add a cube!
					Shape *shape = new Shape("cube.dae");
					shape->loadShape();
					shape->setPosition(glm::vec3(rand() % 10, rand() % 10, rand() % 10));
					addObject(shape);
					break;
				}
				case KeyEvent::KEY_G:
				{
					// mega / regular marble
					if (mPlayer->getRadius() < 1.0f) {
						mPlayer->setRadius(1.0f);
						marbleCubemap->setExtent(glm::vec2(256, 256));
					} else {
						mPlayer->setRadius(0.2f);
						marbleCubemap->setExtent(glm::vec2(64, 64));
					}
					break;
				}
				default:
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

	PhysicsEngine::getEngine()->setStepCallback([&](F64 delta){
		this->loop(delta * 1000.0f);
		this->tick(delta * 1000.0f);
	});

	// onStart
	{
		std::vector<CubeMapTexture::TextureInfo> textures;
		textures.push_back(CubeMapTexture::TextureInfo(std::string("cubemap") + DIR_SEP + "sky0.jpg", CubeMapTexture::PositiveX));
		textures.push_back(CubeMapTexture::TextureInfo(std::string("cubemap") + DIR_SEP + "sky1.jpg", CubeMapTexture::NegativeX));
		textures.push_back(CubeMapTexture::TextureInfo(std::string("cubemap") + DIR_SEP + "sky2.jpg", CubeMapTexture::PositiveY));
		textures.push_back(CubeMapTexture::TextureInfo(std::string("cubemap") + DIR_SEP + "sky3.jpg", CubeMapTexture::NegativeY));
		textures.push_back(CubeMapTexture::TextureInfo(std::string("cubemap") + DIR_SEP + "sky4.jpg", CubeMapTexture::PositiveZ));
		textures.push_back(CubeMapTexture::TextureInfo(std::string("cubemap") + DIR_SEP + "sky5.jpg", CubeMapTexture::NegativeZ));

		CubeMapTexture *cubeMap = new CubeMapTexture(textures);
		Material *skyMaterial = new Material("skybox", cubeMap, GL_TEXTURE0);
		skyMaterial->setShader(mSkyboxShader);
		mSkybox = new Skybox(skyMaterial);
		addObject(mSkybox);
	}

	Camera *camera = new Camera();
	addObject(camera);
	mCamera = camera;

	Sphere *player = new Sphere(glm::vec3(0, 0, 20), 0.2f);
	player->material = new Material("marble.skin");
	player->material->setTexture(marbleCubemap, GL_TEXTURE5);
	player->material->setShader(mInteriorShader);
	mPlayer = player;
	addObject(player);
	
	controlObject = player;

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

		//Update the physics and game items
		PhysicsEngine::getEngine()->simulate(lastDelta);

		render();
		
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
