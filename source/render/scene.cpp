//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/scene.h"
#include "game/gameInterior.h"
#include "game/camera.h"
#include "game/shape.h"
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

	marbleCubemap->generateBuffer(mPlayer->getPosition(), window->getWindowSize() * (S32)pixelDensity, [&](RenderInfo &info) {
		this->renderScene(info);
	}, info);

	//Actually render everything
	renderScene(info);

	if (mDoDebugDraw) {
		glDisable(GL_DEPTH_TEST);
		PhysicsEngine::getEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Everything);
		glEnable(GL_DEPTH_TEST);
	} else {
		PhysicsEngine::getEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Nothing);
	}
}

void Scene::renderScene(RenderInfo &info) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render all the objects in the scene
	for (auto object : mRenderedObjects) {
		//Don't render non-renderable objects (like the camera)
		//Also don't render some objects in reflections (like particles)
		if (object->isRenderable() && (!info.isReflectionPass || object->isReflectable())) {
			object->render(info);
		}
	}

	//Run all render methods
	info.render();
	info.clearRenderMethods();

	//TODO: Clean up shape rendering
	mShapeShader->activate();
	info.loadShader(mShapeShader);

	MODELMGR->render(mShapeShader, info.viewMatrix, info.projectionMatrix);
	mShapeShader->deactivate();

	// check for opengl errors
	GL_CHECKERRORS();
}

void Scene::loop(const F64 &delta) {

}

void Scene::tick(const F64 &delta) {
	if (controlObject) {
		controlObject->updateCamera(movement, delta);
		controlObject->updateMove(movement, delta);
	}

	movement.pitch = 0;
	movement.yaw = 0;

	if (movement.fire) {

	}

	for (auto object : objects) {
		object->updateTick(delta);
	}
}

void Scene::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	mScreenProjectionMatrix = glm::perspective(90.f, aspect, 0.1f, 500.f);

	GLint viewport[4]; //x y w h
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Should be 2x if you have a retina display
	pixelDensity = static_cast<F32>(viewport[2] / size.x);
}

bool Scene::initGL() {
	Shader::initializeShaders();

	marbleCubemap = new CubeMapFramebufferTexture(glm::ivec2(64));
	marbleCubemap->generateBuffer();

	//Window size for viewport
	glm::ivec2 screenSize = window->getWindowSize();
	updateWindowSize(screenSize);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

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
		case Event::KeyDown: {
			KeyEvent::Key key = static_cast<KeyEvent::Key>(static_cast<KeyDownEvent *>(event)->key);
			if (key == mConfig->getKey("forward"))  movement.forward  = true;
			if (key == mConfig->getKey("backward")) movement.backward = true;
			if (key == mConfig->getKey("left"))     movement.left     = true;
			if (key == mConfig->getKey("right"))    movement.right    = true;
			switch (key) {
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
				case KeyEvent::KEY_F:
				{
					mSimulationSpeed *= 0.5f;
					break;
				}
				case KeyEvent::KEY_P:
				{
					mSimulationSpeed *= 2.0f;
					break;
				}
				case KeyEvent::KEY_T:
				{
					mDoDebugDraw = !mDoDebugDraw;
					break;
				}
				case KeyEvent::KEY_U:
				{
					// mbu / regular marble
					if (mPlayer->getRadius() < 0.3f) {
						mPlayer->setRadius(0.3f);
						marbleCubemap->setExtent(glm::vec2(128, 128));
					}
					else {
						mPlayer->setRadius(0.2f);
						marbleCubemap->setExtent(glm::vec2(64, 64));
					}
					break;
				}
				case KeyEvent::KEY_ESCAPE:
				{
					window->lockCursor(false);
					captureMouse = false;
					break;
				}
				default:
					break;
			}
			break;
		}
		case Event::KeyUp: {
			KeyEvent::Key key = static_cast<KeyEvent::Key>(static_cast<KeyDownEvent *>(event)->key);
			if (key == mConfig->getKey("forward"))  movement.forward  = false;
			if (key == mConfig->getKey("backward")) movement.backward = false;
			if (key == mConfig->getKey("left"))     movement.left     = false;
			if (key == mConfig->getKey("right"))    movement.right    = false;
			switch (key) {
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
		}
		//Mouse for rotation
		case Event::MouseMove:
			if (captureMouse) {
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

			if (((MouseDownEvent *)event)->button == 1) { //Left mouse: click
				window->lockCursor(true);
				captureMouse = true;
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
	mDoDebugDraw = false;

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
	
	// destroy all shaders
	Shader::destroyAllShaders();

	//Destroy the SDL
	window->destroyContext();
}

void Scene::addObject(GameObject *object) {
	objects.push_back(object);

	// For rendered objects.
	auto *renderedObject = dynamic_cast<RenderedObject*>(object);
	if (renderedObject != nullptr) {
		mRenderedObjects.push_back(renderedObject);

		// fire callback
		renderedObject->onAddToScene();
	}
}

GameObject* Scene::findGameObject(const std::string &name) {
	// O(n)
	// TODO: store objects in a hash map or something.
	for (const auto obj : objects) {
		if (obj->getName() == name)
			return obj;
	}
	return nullptr;
}

void Scene::run() {
	Event *eventt;

	F64 lastDelta = 0;
	
	F64 counter = 0;
	U32 fpsCounter = 0;

	mSimulationSpeed = 1.0f;

	PhysicsEngine::getEngine()->setStepCallback([&](F64 delta){
		if (mSimulationSpeed == 1.0f) {
		this->loop(delta);
		this->tick(delta);
		}
	});

	// onStart

	//Create camera
	{
		Camera *camera = new Camera();
		addObject(camera);
		mCamera = camera;
	}

	//Create player
	{
		Sphere *player = new Sphere(glm::vec3(0, 0, 20), 0.2f);
		Material *material = new Material("marble.skin");
		material->setTexture(marbleCubemap, GL_TEXTURE3);
		material->setShader(Shader::getShaderByName("Sphere"));
		player->setMaterial(material);
		mPlayer = player;
		addObject(player);
	}
	
	controlObject = mPlayer;

	//Maybe this is a good idea
	PhysicsEngine::getEngine()->setRunning(true);

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
		PhysicsEngine::getEngine()->simulate(lastDelta * mSimulationSpeed);

		if (mSimulationSpeed != 1.0f) {
			this->loop(lastDelta * mSimulationSpeed);
			this->tick(lastDelta * mSimulationSpeed);
		}

		render();
		
		//Flip buffers
		window->swapBuffers();

		//Profiling
		if (printFPS) {
			counter += lastDelta;
			fpsCounter++;
			if (counter >= 1.f) {
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
