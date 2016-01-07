//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/renderWorld.h"
#include "game/gameInterior.h"
#include "game/camera.h"
#include "game/shape.h"
#include "game/skybox.h"
#include "render/util.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>
#include "network/client.h"

glm::mat4 RenderInfo::inverseRotMat = glm::rotate(glm::mat4x4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0));

/// The amount of time that has to pass before a tick happens.
/// Default is 16.6667 ms which means we tick at 60 frames per second
#define TICK_MS 16.6666666666666667

RenderWorld::RenderWorld(PhysicsEngine *physics) : World(physics) {
	mShapeShader = nullptr;
	mControlObject = nullptr;
}

RenderWorld::~RenderWorld() {
	for (auto object : mObjects) {
		delete object;
	}
	delete mWindow;
	delete mTimer;
}

void RenderWorld::render() {
	//Get the camera transform from the marble
	glm::mat4 cameraTransform;
	glm::vec3 cameraPosition;
	if (mControlObject)
		mControlObject->getCameraPosition(cameraTransform, cameraPosition);

	//Camera
	glm::mat4 viewMatrix = glm::mat4x4(1);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
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

	mMarbleCubemap->generateBuffer(mPlayer->getPosition(), mWindow->getWindowSize() * (S32)mPixelDensity, [&](RenderInfo &info) {
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

void RenderWorld::renderScene(RenderInfo &info) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render all the mObjects in the scene
	for (auto object : mRenderedObjects) {
		//Don't render non-renderable mObjects (like the camera)
		//Also don't render some mObjects in reflections (like particles)
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

void RenderWorld::loop(const F64 &delta) {

}

void RenderWorld::tick(const F64 &delta) {
	if (mControlObject) {
		mControlObject->updateCamera(mMovement, delta);
		mControlObject->updateMove(mMovement, delta);
	}

	mMovement.pitch = 0;
	mMovement.yaw = 0;

	if (mMovement.fire) {

	}

	for (auto object : mObjects) {
		object->updateTick(delta);
	}
}

void RenderWorld::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	mScreenProjectionMatrix = glm::perspective(glm::radians(90.f), aspect, 0.1f, 500.f);

	GLint viewport[4]; //x y w h
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Should be 2x if you have a retina display
	mPixelDensity = static_cast<F32>(viewport[2] / size.x);
}

bool RenderWorld::initGL() {
	Shader::initializeShaders();
	mShapeShader = Shader::getShaderByName("Model");

	mMarbleCubemap = new CubeMapFramebufferTexture(glm::ivec2(64));
	mMarbleCubemap->generateBuffer();

	//Window size for viewport
	glm::ivec2 screenSize = mWindow->getWindowSize();
	updateWindowSize(screenSize);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "Error in GL init: %d", err);
		return false;
	}
	return true;
}

void RenderWorld::performClick(S32 mouseX, S32 mouseY) {
	/*
	glm::ivec2 screenSize = mWindow->getWindowSize();
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

void RenderWorld::handleEvent(PlatformEvent *event) {
	//Key events, movement
	switch (event->getType()) {
		//Quit
		case PlatformEvent::Quit:
			mRunning = false;
			break;
		case PlatformEvent::KeyDown: {
			KeyEvent::Key key = static_cast<KeyEvent::Key>(static_cast<KeyDownEvent *>(event)->key);
			if (key == mConfig->getKey("forward"))  mMovement.forward  = true;
			if (key == mConfig->getKey("backward")) mMovement.backward = true;
			if (key == mConfig->getKey("left"))     mMovement.left     = true;
			if (key == mConfig->getKey("right"))    mMovement.right    = true;
			switch (key) {
				case KeyEvent::KEY_UP:    mMovement.pitchUp   = true; break;
				case KeyEvent::KEY_DOWN:  mMovement.pitchDown = true; break;
				case KeyEvent::KEY_LEFT:  mMovement.yawLeft   = true; break;
				case KeyEvent::KEY_RIGHT: mMovement.yawRight  = true; break;
				case KeyEvent::KEY_SPACE: mMovement.jump      = true; break;
				case KeyEvent::KEY_V: mWindow->toggleVsync(); break;
				case KeyEvent::KEY_Q:
				{
					mMovement.fire = true;

					// make a new sphere!
					Sphere *sphere = new Sphere(glm::vec3(0, 0, 30), 0.75);
					addObject(sphere);

					break;
				}
				case KeyEvent::KEY_C:
				{
					// swap control mObjects!
					if (mControlObject == mPlayer)
						mControlObject = mCamera;
					else
						mControlObject = mPlayer;
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
						mMarbleCubemap->setExtent(glm::vec2(256, 256));
					} else {
						mPlayer->setRadius(0.2f);
						mMarbleCubemap->setExtent(glm::vec2(64, 64));
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
						mMarbleCubemap->setExtent(glm::vec2(128, 128));
					}
					else {
						mPlayer->setRadius(0.2f);
						mMarbleCubemap->setExtent(glm::vec2(64, 64));
					}
					break;
				}
				case KeyEvent::KEY_ESCAPE:
				{
					mWindow->lockCursor(false);
					mCaptureMouse = false;
					break;
				}
				default:
					break;
			}
			break;
		}
		case PlatformEvent::KeyUp: {
			KeyEvent::Key key = static_cast<KeyEvent::Key>(static_cast<KeyDownEvent *>(event)->key);
			if (key == mConfig->getKey("forward"))  mMovement.forward  = false;
			if (key == mConfig->getKey("backward")) mMovement.backward = false;
			if (key == mConfig->getKey("left"))     mMovement.left     = false;
			if (key == mConfig->getKey("right"))    mMovement.right    = false;
			switch (key) {
				case KeyEvent::KEY_UP:    mMovement.pitchUp   = false; break;
				case KeyEvent::KEY_DOWN:  mMovement.pitchDown = false; break;
				case KeyEvent::KEY_LEFT:  mMovement.yawLeft   = false; break;
				case KeyEvent::KEY_RIGHT: mMovement.yawRight  = false; break;
				case KeyEvent::KEY_SPACE: mMovement.jump      = false; break;
				case KeyEvent::KEY_Q:     mMovement.fire      = false; break;
				default:
					break;
			}
			break;
		}
		//Mouse for rotation
		case PlatformEvent::MouseMove:
			if (mCaptureMouse) {
				mMovement.yaw += (GLfloat)((MouseMoveEvent *)event)->delta.x;
				mMovement.pitch += (GLfloat)((MouseMoveEvent *)event)->delta.y;
			}
			break;
		case PlatformEvent::MouseDown:
			switch (((MouseDownEvent *)event)->button) {
				case 1: mouseButtons.left   = true; break;
				case 2: mouseButtons.middle = true; break;
				case 3: mouseButtons.right  = true; break;
				default: break;
			}

			if (((MouseDownEvent *)event)->button == 1) { //Left mouse: click
				mWindow->lockCursor(true);
				mCaptureMouse = true;
				performClick(((MouseDownEvent *)event)->position.x, ((MouseDownEvent *)event)->position.y);
			}
			break;
		case PlatformEvent::MouseUp:
			switch (((MouseDownEvent *)event)->button) {
				case 1: mouseButtons.left   = false; break;
				case 2: mouseButtons.middle = false; break;
				case 3: mouseButtons.right  = false; break;
				default: break;
			}
			break;
		case PlatformEvent::WindowFocus:
			mShouldSleep = false;
			break;
		case PlatformEvent::WindowBlur:
			mShouldSleep = true;
			break;
		case PlatformEvent::WindowResize:
			updateWindowSize(static_cast<WindowResizeEvent *>(event)->newSize);
			break;
		default:
			break;
	}
}

bool RenderWorld::init() {
	mRunning = true;
	mShouldSleep = false;
	mDoDebugDraw = false;

	if (!mWindow->createContext()) {
		return false;
	}

	//Initialize OpenGL
	if (!initGL()) {
		return false;
	}

	mCaptureMouse = true;

	return true;
}

void RenderWorld::cleanup() {
	delete mTimer;
	
	// destroy all shaders
	Shader::destroyAllShaders();

	//Destroy the SDL
	mWindow->destroyContext();
}

void RenderWorld::addObject(GameObject *object) {
	mObjects.push_back(object);

	// For rendered objects.
	auto *renderedObject = dynamic_cast<RenderedObject*>(object);
	if (renderedObject != nullptr) {
		mRenderedObjects.push_back(renderedObject);

		// fire callback
		renderedObject->onAddToScene();
	}
}

GameObject* RenderWorld::findGameObject(const std::string &name) {
	// O(n)
	// TODO: store objects in a hash map or something.
	for (const auto obj : mObjects) {
		if (obj->getName() == name)
			return obj;
	}
	return nullptr;
}

void RenderWorld::run() {
	PlatformEvent *eventt;

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

	// NETWORKING MWHAHAHAHAH
	Client client("127.0.0.1", 28000);
	client.connect();

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
		material->setTexture(mMarbleCubemap, GL_TEXTURE3);
		material->setShader(Shader::getShaderByName("Sphere"));
		player->setMaterial(material);
		mPlayer = player;
		addObject(player);
	}
	
	mControlObject = mPlayer;

	//Main loop
	while (mRunning) {
		//Profiling
		mTimer->start();
		
		if (mShouldSleep) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		//Input
		while (mWindow->pollEvents(eventt)) {
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

		// le networking
		client.pollEvents();

		render();
		
		//Flip buffers
		mWindow->swapBuffers();

		//Profiling
		if (mPrintFPS) {
			counter += lastDelta;
			fpsCounter++;
			if (counter >= 1.f) {
				F32 mspf = 1000.0f / fpsCounter;
				std::string title = "FPS: " + std::to_string(fpsCounter) + " mspf: " + std::to_string(mspf);
				mWindow->setWindowTitle(title.c_str());
				
				counter = 0.0;
				fpsCounter = 0;
			}
		}
		
		//Count how long a frame took
		// calculate delta of this elapsed frame.
		mTimer->end();

		lastDelta = mTimer->getDelta();
	}

	// cleanup client.
	client.disconnect();

	//Clean up (duh)
	cleanup();
}
