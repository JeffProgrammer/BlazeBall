//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "renderer.h"

#include "render/renderWorld.h"
#include "network/client.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Client *client) : mClient(client) {
	mDoDebugDraw = false;
}

Renderer::~Renderer() {
	//Destroy the SDL
	mWindow->destroyContext();

	delete mWindow;

	// destroy all shaders
	Shader::destroyAllShaders();

	mRocketContext->UnloadAllDocuments();
	mRocketContext->RemoveReference();
	Rocket::Core::Shutdown();
}

void Renderer::render(const F64 &delta) {
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

	RenderInfo info;
	info.projectionMatrix = mScreenProjectionMatrix;
	info.windowSize = mWindow->getWindowSize();
	info.pixelDensity = mPixelDensity;

	//TODO: Have this in an element
	dynamic_cast<RenderWorld *>(mClient->getWorld())->render(info);

	// render GUI
	mRocketContext->Render();
	mRocketContext->Update();

	//Flip buffers
	mWindow->swapBuffers();


	//TODO: Move all of this into client
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

	//TODO: Fix this
//	if (mDoDebugDraw) {
//		glDisable(GL_DEPTH_TEST);
//		getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Everything);
//		glEnable(GL_DEPTH_TEST);
//	} else {
//		getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Nothing);
//	}
}


void Renderer::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	mScreenProjectionMatrix = glm::perspective(glm::radians(90.f), aspect, 0.1f, 500.f);

	GLint viewport[4]; //x y w h
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Should be 2x if you have a retina display
	mPixelDensity = static_cast<F32>(viewport[2] / size.x);
}

bool Renderer::init() {
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

bool Renderer::initGL() {
	Shader::initializeShaders();

	//Window size for viewport
	glm::ivec2 screenSize = mWindow->getWindowSize();
	updateWindowSize(screenSize);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	// Initialize the gui library, librocket
	// TODO: move this out of a 3D thing
	mGuiInterface = new GuiInterface();
	mGuiRenderInterface = new GuiRenderInterface(mWindow);
	Rocket::Core::SetSystemInterface(mGuiInterface);
	Rocket::Core::SetRenderInterface(mGuiRenderInterface);
	if (!Rocket::Core::Initialise()) {
		IO::printf("Unable to initialize rocket.\n");
		return false;
	}

	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Bold.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-BoldItalic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Italic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");

	// Initialize base gui
	glm::ivec2 dims = mWindow->getWindowSize();
	mRocketContext = Rocket::Core::CreateContext("tutorial", Rocket::Core::Vector2i(dims.x, dims.y));
	mRocketDocument = mRocketContext->LoadDocument("tutorial.rml");
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

void Renderer::performClick(S32 mouseX, S32 mouseY) {
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

void Renderer::handleEvent(PlatformEvent *event) {
	//Key events, movement
	switch (event->getType()) {
			//Quit
		case PlatformEvent::Quit:
			mClient->stop();
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
