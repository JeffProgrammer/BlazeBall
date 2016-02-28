//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_RENDERER_H_
#define _RENDER_RENDERER_H_

#include <Rocket/Core.h>
#include <Rocket/Core/Input.h>
#include <Rocket/Core/ElementDocument.h>

#include "platform/platformGL.h"
#include "base/math.h"
#include "base/types.h"
#include "base/io.h"
#include "physics/physicsEngine.h"
#include "render/shader.h"
#include "platform/platformWindow.h"
#include "platform/platformEvent.h"
#include "base/config.h"
#include "render/renderInfo.h"
#include "gui/renderInterface.h"
#include "gui/systemInterface.h"
#include "gui/guiDocument.h"

class Client;
class Renderer {
protected:
	Client *mClient;
	PlatformWindow *mWindow;
	Config *mConfig;

	Rocket::Core::Context *mRocketContext;

	GuiDocument *mCurrentDocument;

	struct {
		bool left;
		bool middle;
		bool right;
	} mouseButtons;

	bool mCaptureMouse;
	bool mPrintFPS;
	bool mShouldSleep;

	glm::mat4 mScreenProjectionMatrix;
public:
	Renderer(Client *client);
	~Renderer();

	PlatformWindow *getWindow() const { return mWindow; }
	void setWindow(PlatformWindow *window) { mWindow = window; }

	bool getCaptureMouse() { return mCaptureMouse; }
	void setCaptureMouse(bool capture) {
		mCaptureMouse = capture;
		mWindow->lockMouse(capture);
	}

	void render(const F64 &delta);

	bool initGUI();
	bool initGL();
	bool init();
	void handleEvent(PlatformEvent *event);

	Rocket::Core::ElementDocument *loadDocument(const std::string &path);
	void setCurrentDocument(GuiDocument *document);
	void setCurrentDocument(const std::string &name);

	void updateWindowSize(const glm::ivec2 &size);
	RenderInfo getRenderInfo(const glm::ivec2 &size);
};

#endif _RENDER_RENDERER_H_