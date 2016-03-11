//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "renderer.h"

#include "render/renderWorld.h"
#include "network/client.h"
#include <glm/gtc/matrix_transform.hpp>

#include "gui/documents/mainMenuDocument.h"
#include "gui/documents/gameDocument.h"

Renderer::Renderer(Client *client) : mClient(client), mFPS(0), mCaptureMouse(false) {
	mRocketContext = nullptr;
}

Renderer::~Renderer() {
	//Destroy the SDL
	mWindow->destroyContext();

	delete mWindow;

	// destroy all shaders
	Shader::destroyAllShaders();

	mRocketContext->UnloadAllDocuments();
	mRocketContext->RemoveReference();
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update GUI before render
	mCurrentDocument->onRender();

	// render GUI
	mRocketContext->Render();
	mRocketContext->Update();

	GL_CHECKERRORS();

	//Flip buffers
	mWindow->swapBuffers();
}

RenderInfo Renderer::getRenderInfo(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;

	//Weird behavior with windows that are too small
	if (size.x == 0 || size.y == 0)
		aspect = 1;

	RenderInfo info;
	info.projectionMatrix = glm::perspective(glm::radians(90.f), aspect, 0.1f, 500.f);
	info.viewport.size = mWindow->getWindowSize();
	info.pixelDensity = glGetPixelDensityEXT();

	return info;
}

void Renderer::updateWindowSize(const glm::ivec2 &size) {
	GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;
	mScreenProjectionMatrix = glm::perspective(glm::radians(90.f), aspect, 0.1f, 500.f);

	// We use this instead of our extension because we don't have a way
	// in software to get the pixel density.
	GLint viewport[4]; //x y w h
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Should be 2x if you have a retina display
	glSetPixelDensityEXT(GLfloat(viewport[2] / size.x));

	if (mRocketContext != nullptr)
		mRocketContext->SetDimensions(Rocket::Core::Vector2i(size.x, size.y));
}

bool Renderer::init() {
	if (!mWindow->createContext()) {
		return false;
	}

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "Error in GL init: %d", err);
		return false;
	}
	return true;
}

bool Renderer::initGUI() {
	glm::ivec2 screenSize = mWindow->getWindowSize();

	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Bold.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-BoldItalic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Italic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");

	// Initialize base gui
	mRocketContext = Rocket::Core::CreateContext("game", Rocket::Core::Vector2i(screenSize.x, screenSize.y));
	mCurrentDocument = new MainMenuDocument("MainMenu", this, loadDocument("menu.rml"));

	//Load the game for later usage
	new GameDocument("Game", this, loadDocument("game.rml"));

	if (mCurrentDocument) {
		setCurrentDocument(mCurrentDocument);
	} else {
		IO::printf("Could not load menu\n");
		return false;
	}

	return true;
}

Rocket::Core::ElementDocument *Renderer::loadDocument(const std::string &path) {
	Rocket::Core::ElementDocument *rdoc = mRocketContext->LoadDocument(path.c_str());
	if (rdoc) {
		return rdoc;
	} else {
		return nullptr;
	}
}

void Renderer::setCurrentDocument(GuiDocument *document) {
	mCurrentDocument->getDocument()->Hide();
	mCurrentDocument->onSleep();

	mCurrentDocument = document;

	document->getDocument()->Show();
	document->onWake();
}

void Renderer::setCurrentDocument(const std::string &name) {
	GuiDocument *doc = GuiDocument::getDocument(name);
	setCurrentDocument(doc);
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
			switch (key) {
				case KeyEvent::KEY_V: mWindow->toggleVsync(); break;
				case KeyEvent::KEY_T:
				{
					//TODO
//					mDoDebugDraw = !mDoDebugDraw;
					break;
				}
				default:
					break;
			}
			mRocketContext->ProcessKeyDown(GuiInterface::translateKey(key), 0);
			break;
		}
		case PlatformEvent::KeyUp: {
			KeyEvent::Key key = static_cast<KeyEvent::Key>(static_cast<KeyDownEvent *>(event)->key);
			mRocketContext->ProcessKeyUp(GuiInterface::translateKey(key), 0);
			break;
		}
		case PlatformEvent::MouseMove:
			if (mCaptureMouse) {
				Rocket::Core::Dictionary dict;
				dict.Set("mouse_x", static_cast<MouseMoveEvent *>(event)->position.x);
				dict.Set("mouse_y", static_cast<MouseMoveEvent *>(event)->position.y);
				dict.Set("movement_x", static_cast<MouseMoveEvent *>(event)->delta.x);
				dict.Set("movement_y", static_cast<MouseMoveEvent *>(event)->delta.y);

				mCurrentDocument->getDocument()->DispatchEvent("mousemove", dict, true);
			} else {
				mRocketContext->ProcessMouseMove(static_cast<MouseMoveEvent *>(event)->position.x, static_cast<MouseMoveEvent *>(event)->position.y, 0);
			}
			break;
		case PlatformEvent::MouseDown:
			mRocketContext->ProcessMouseButtonDown(GuiInterface::translateMouseButton(static_cast<MouseDownEvent *>(event)->button), 0);
			break;
		case PlatformEvent::MouseUp:
			mRocketContext->ProcessMouseButtonUp(GuiInterface::translateMouseButton(static_cast<MouseDownEvent *>(event)->button), 0);
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
