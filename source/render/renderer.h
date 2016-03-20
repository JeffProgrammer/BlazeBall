//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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
	RenderInfo getRenderInfo(const glm::ivec2 &size);
};

#endif _RENDER_RENDERER_H_