//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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

class Client;
class Renderer {
protected:
	Client *mClient;
	PlatformWindow *mWindow;
	Config *mConfig;

	Rocket::Core::Context *mRocketContext;
	Rocket::Core::ElementDocument *mRocketDocument;

	struct {
		bool left;
		bool middle;
		bool right;
	} mouseButtons;

	bool mCaptureMouse;
	bool mPrintFPS;
	bool mShouldSleep;

	F32 mPixelDensity;

	glm::mat4 mScreenProjectionMatrix;
public:
	Renderer(Client *client);
	~Renderer();

	PlatformWindow *getWindow() const { return mWindow; }
	void setWindow(PlatformWindow *window) { mWindow = window; }

	void render(const F64 &delta);

	bool initGUI();
	bool initGL();
	bool init();
	void handleEvent(PlatformEvent *event);

	void updateWindowSize(const glm::ivec2 &size);
};

#endif