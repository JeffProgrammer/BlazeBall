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
	mRocketContext = nullptr;
	mDoDebugDraw = false;
	mConfig = new Config("config.txt");
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

	if (mDoDebugDraw) {
		glDisable(GL_DEPTH_TEST);
		mClient->getWorld()->getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Everything);
		glEnable(GL_DEPTH_TEST);
	} else {
		mClient->getWorld()->getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Nothing);
	}
}


void Renderer::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	mScreenProjectionMatrix = glm::perspective(glm::radians(90.f), aspect, 0.1f, 500.f);

	// We use this instead of our extension because we don't have a way
	// in software to get the pixel density.
	GLint viewport[4]; //x y w h
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Should be 2x if you have a retina display
	mPixelDensity = static_cast<F32>(viewport[2] / size.x);

	if (mRocketContext != nullptr)
		mRocketContext->SetDimensions(Rocket::Core::Vector2i(size.x, size.y));
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
	mRocketContext = Rocket::Core::CreateContext("tutorial", Rocket::Core::Vector2i(screenSize.x, screenSize.y));
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

void Renderer::handleEvent(PlatformEvent *event) {
	//Key events, movement
	switch (event->getType()) {
			//Quit
		case PlatformEvent::Quit:
			mClient->stop();
			break;
		case PlatformEvent::KeyDown: {
			KeyEvent::Key key = static_cast<KeyEvent::Key>(static_cast<KeyDownEvent *>(event)->key);
			if (key == mConfig->getKey("forward"))  mClient->getMovement().forward  = true;
			if (key == mConfig->getKey("backward")) mClient->getMovement().backward = true;
			if (key == mConfig->getKey("left"))     mClient->getMovement().left     = true;
			if (key == mConfig->getKey("right"))    mClient->getMovement().right    = true;
			switch (key) {
				case KeyEvent::KEY_UP:    mClient->getMovement().pitchUp   = true; break;
				case KeyEvent::KEY_DOWN:  mClient->getMovement().pitchDown = true; break;
				case KeyEvent::KEY_LEFT:  mClient->getMovement().yawLeft   = true; break;
				case KeyEvent::KEY_RIGHT: mClient->getMovement().yawRight  = true; break;
				case KeyEvent::KEY_SPACE: mClient->getMovement().jump      = true; break;
				case KeyEvent::KEY_V: mWindow->toggleVsync(); break;
				case KeyEvent::KEY_Q: mClient->getMovement().fire = true; break;
				case KeyEvent::KEY_T:
				{
					mDoDebugDraw = !mDoDebugDraw;
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
			if (key == mConfig->getKey("forward"))  mClient->getMovement().forward  = false;
			if (key == mConfig->getKey("backward")) mClient->getMovement().backward = false;
			if (key == mConfig->getKey("left"))     mClient->getMovement().left     = false;
			if (key == mConfig->getKey("right"))    mClient->getMovement().right    = false;
			switch (key) {
				case KeyEvent::KEY_UP:    mClient->getMovement().pitchUp   = false; break;
				case KeyEvent::KEY_DOWN:  mClient->getMovement().pitchDown = false; break;
				case KeyEvent::KEY_LEFT:  mClient->getMovement().yawLeft   = false; break;
				case KeyEvent::KEY_RIGHT: mClient->getMovement().yawRight  = false; break;
				case KeyEvent::KEY_SPACE: mClient->getMovement().jump      = false; break;
				case KeyEvent::KEY_Q:     mClient->getMovement().fire      = false; break;
				default:
					break;
			}
			break;
		}
			//Mouse for rotation
		case PlatformEvent::MouseMove:
			if (mCaptureMouse) {
				mClient->getMovement().yaw += (GLfloat)((MouseMoveEvent *)event)->delta.x;
				mClient->getMovement().pitch += (GLfloat)((MouseMoveEvent *)event)->delta.y;
			} else {
				mRocketContext->ProcessMouseMove(static_cast<MouseMoveEvent *>(event)->position.x, static_cast<MouseMoveEvent *>(event)->position.x, 0);
			}
			break;
		case PlatformEvent::MouseDown:
			switch (((MouseDownEvent *)event)->button) {
				case MouseButton::MOUSE_BUTTON_LEFT: mouseButtons.left   = true; break;
				case MouseButton::MOUSE_BUTTON_MIDDLE: mouseButtons.middle = true; break;
				case MouseButton::MOUSE_BUTTON_RIGHT: mouseButtons.right  = true; break;
				default: break;
			}
			if (!mCaptureMouse) {
				mRocketContext->ProcessMouseButtonDown(static_cast<MouseDownEvent *>(event)->button, 0);
			}
			break;
		case PlatformEvent::MouseUp:
			switch (((MouseDownEvent *)event)->button) {
				case MouseButton::MOUSE_BUTTON_LEFT: mouseButtons.left   = false; break;
				case MouseButton::MOUSE_BUTTON_MIDDLE: mouseButtons.middle = false; break;
				case MouseButton::MOUSE_BUTTON_RIGHT: mouseButtons.right  = false; break;
				default: break;
			}
			if (!mCaptureMouse) {
				mRocketContext->ProcessMouseButtonUp(static_cast<MouseDownEvent *>(event)->button, 0);
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
