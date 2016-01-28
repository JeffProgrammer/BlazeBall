//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/renderWorld.h"
#include "game/gameInterior.h"
#include "game/camera.h"
//#include "game/shape.h"
#include "game/skybox.h"
#include "render/util.h"
#include "network/client.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>
#include <glm/gtx/vector_angle.hpp>

glm::mat4 RenderInfo::inverseRotMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

/// The amount of time that has to pass before a tick happens.
/// Default is 16.6667 ms which means we tick at 60 frames per second
#define TICK_MS 16.6666666666666667

RenderWorld::RenderWorld(PhysicsEngine *physics, ScriptEngine *script) : World(physics, script) {
//	mShapeShader = nullptr;

	mDoDebugDraw = false;
}

RenderWorld::~RenderWorld() {
	delete mWindow;
	delete mTimer;

	// destroy all shaders
	Shader::destroyAllShaders();

	//Destroy the SDL
	mWindow->destroyContext();
}

void RenderWorld::render() {
	//Get the camera transform from the marble
	glm::mat4 cameraTransform(1.0f);
	glm::vec3 cameraPosition(0.0f);

	if (mClient->getControlObject())
		mClient->getControlObject()->getCameraPosition(cameraTransform, cameraPosition);

	//Camera
	glm::mat4 viewMatrix = glm::mat4(1.0f);
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

	glm::vec3 position = (mClient->getControlObject() ? mClient->getControlObject()->getPosition() : glm::vec3(0));
	mMarbleCubemap->generateBuffer(position, mWindow->getWindowSize() * (S32)mPixelDensity, [&](RenderInfo &info) {
		this->renderScene(info);
	}, info);

	//Actually render everything
	renderScene(info);

	if (mDoDebugDraw) {
		glDisable(GL_DEPTH_TEST);
		getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Everything);
		glEnable(GL_DEPTH_TEST);
	} else {
		getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Nothing);
	}
}

void RenderWorld::renderScene(RenderInfo &info) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
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
//	mShapeShader->activate();
//	info.loadShader(mShapeShader);
//
//	MODELMGR->render(mShapeShader, info.viewMatrix, info.projectionMatrix);
//	mShapeShader->deactivate();

	// check for opengl errors
	GL_CHECKERRORS();
}

void RenderWorld::loop(const F64 &delta) {
	PlatformEvent *eventt;

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

	World::loop(delta);

	render();

	// render GUI
	mRocketContext->Render();
	mRocketContext->Update();

	//Flip buffers
	mWindow->swapBuffers();
}

void RenderWorld::tick(const F64 &delta) {
	World::tick(delta);

	GameObject *control = mClient->getControlObject();
	if (control) {
		// temp code
		auto event = std::make_shared<NetClientGhostUpdateEvent>(mClient, control);
		mClient->sendEvent(event, ENetPacketFlag::ENET_PACKET_FLAG_UNSEQUENCED);

		if (control) {
			control->updateCamera(mMovement, delta);
			control->updateMove(mMovement, delta);
		}
	}

	mMovement.pitch = 0;
	mMovement.yaw = 0;

	if (mMovement.fire) {

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

bool RenderWorld::init() {

	if (!mWindow->createContext()) {
		return false;
	}

	mCaptureMouse = true;
	mWindow->lockCursor(true);

	//Initialize OpenGL
	if (!initGL()) {
		return false;
	}

	return true;
}

bool RenderWorld::initGL() {
	Shader::initializeShaders();
//	mShapeShader = Shader::getShaderByName("Model");

	mMarbleCubemap = new CubeMapFramebufferTexture(glm::ivec2(64));
	mMarbleCubemap->generateBuffer();

	//Window size for viewport
	glm::ivec2 screenSize = mWindow->getWindowSize();
	updateWindowSize(screenSize);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	// Initialize the gui library, librocket
	// TODO: move this out of a 3D thing.
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Bold.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-BoldItalic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Italic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");

	mGuiInterface = new GuiInterface();
	mGuiRenderInterface = new GuiRenderInterface(mWindow);
	Rocket::Core::SetSystemInterface(mGuiInterface);
	Rocket::Core::SetRenderInterface(mGuiRenderInterface);
	if (!Rocket::Core::Initialise()) {
		IO::printf("Unable to initialize rocket.\n");
		return false;
	}

	// Initialize base gui
	glm::ivec2 dims = mWindow->getWindowSize();
	mRocketContext = Rocket::Core::CreateContext("demo", Rocket::Core::Vector2i(dims.x, dims.y));
	mRocketDocument = mRocketContext->LoadDocument("demo.rml");
	if (mRocketDocument) {
		mRocketDocument->Show();
		mRocketDocument->RemoveReference();
	} else {
		IO::printf("Unable to show document demo.rml\n");
	}

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
				case KeyEvent::KEY_Q: mMovement.fire = true; break;
				case KeyEvent::KEY_C:
				{
					// swap control mObjects!
					//TODO: Server this
//					if (mControlObject == mPlayer)
//						mControlObject = mCamera;
//					else
//						mControlObject = mPlayer;
					break;
				}
				case KeyEvent::KEY_M:
				{
					// add a cube!
//					Shape *shape = new Shape(this, "cube.dae");
//					shape->loadShape();
//					shape->setPosition(glm::vec3(rand() % 10, rand() % 10, rand() % 10));
//					addObject(shape);
					break;
				}
				case KeyEvent::KEY_G:
				{
					// mega / regular marble
					//TODO: Server this
//					if (mPlayer->getRadius() < 1.0f) {
//						mPlayer->setRadius(1.0f);
//						mMarbleCubemap->setExtent(glm::vec2(256, 256));
//					} else {
//						mPlayer->setRadius(0.2f);
//						mMarbleCubemap->setExtent(glm::vec2(64, 64));
//					}
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
					//TODO: Server this
//					if (mPlayer->getRadius() < 0.3f) {
//						mPlayer->setRadius(0.3f);
//						mMarbleCubemap->setExtent(glm::vec2(128, 128));
//					}
//					else {
//						mPlayer->setRadius(0.2f);
//						mMarbleCubemap->setExtent(glm::vec2(64, 64));
//					}
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
				case MouseButton::MOUSE_BUTTON_LEFT: mouseButtons.left   = true; break;
				case MouseButton::MOUSE_BUTTON_MIDDLE: mouseButtons.middle = true; break;
				case MouseButton::MOUSE_BUTTON_RIGHT: mouseButtons.right  = true; break;
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
				case MouseButton::MOUSE_BUTTON_LEFT: mouseButtons.left   = false; break;
				case MouseButton::MOUSE_BUTTON_MIDDLE: mouseButtons.middle = false; break;
				case MouseButton::MOUSE_BUTTON_RIGHT: mouseButtons.right  = false; break;
				default: break;
			}
			break;
		case PlatformEvent::WindowFocus:
			mShouldSleep = false;
			mWindow->lockCursor(true);
			mCaptureMouse = true;
			break;
		case PlatformEvent::WindowBlur:
			mShouldSleep = true;
			mWindow->lockCursor(false);
			mCaptureMouse = false;
			break;
		case PlatformEvent::WindowResize:
			updateWindowSize(static_cast<WindowResizeEvent *>(event)->newSize);
			break;
		default:
			break;
	}
}

void RenderWorld::addObject(GameObject *object) {
	World::addObject(object);

	// For rendered objects.
	auto *renderedObject = dynamic_cast<RenderedObject*>(object);
	if (renderedObject != nullptr) {
		mRenderedObjects.push_back(renderedObject);
	}
}
