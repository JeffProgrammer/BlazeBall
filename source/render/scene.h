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


#ifndef scene_h
#define scene_h

#include <stdio.h>
#include "base/math.h"
#include "base/types.h"
#include "base/io.h"
#include <dif/objects/dif.h>
#include "render/bitmap/texture.h"
#include "game/sphere.h"
#include "physics/physicsEngine.h"
#include "render/shader.h"
#include "render/scene.h"
#include "platform/window.h"
#include "platform/event.h"
#include "platform/timer.h"
#include <vector>
#include "game/movement.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/GLU.h>
#include <GL/glew.h>
#endif

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

class Scene {
protected:
	void (*loopCallback)();

	bool running;
	bool printFPS = true;

	//F32 maxFPS = 60.0f;

	GLuint mvpMatrixLocation;
	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;

	GLuint modelView3Location;

	GLuint lightDirectionLocation;
	GLuint lightColorLocation;
	GLuint ambientColorLocation;
	GLuint sunPositionLocation;
	GLuint sunPowerLocation;
	GLuint specularExponentLocation;

	glm::mat4x4 projectionMatrix, modelMatrix, viewMatrix;

	struct {
		bool hasSelection;
		U32 surfaceIndex;
		GameInterior *interior;
	} selection;

	bool captureMouse = false;
	struct {
		bool left;
		bool middle;
		bool right;
	} mouseButtons;
	Movement movement;

public:
	const glm::vec4 lightColor     = glm::vec4(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const glm::vec3 lightDirection = glm::vec3(0.60f, 0.40f, 1.0f);
	const glm::vec4 ambientColor   = glm::vec4(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const glm::vec3 sunPosition    = glm::vec3(100.0f, 75.0f, 100.0f);
	const F32 sunPower           = 1000.0f;
	const U32 specularExponent   = 7;

	std::vector<GameInterior *>interiors;
	Sphere *sphere;

	GameObject *controlObject;

	Window *window;
	
	Timer *mTimer;
	
	bool mShouldSleep;

	static Scene *getSingleton() {
		static Scene *singleton;
		if (!singleton)
			singleton = new Scene();
		return singleton;
	}
	void setLoopCallback(void (*callback)()) {
		loopCallback = callback;
	}

	void render();
	void loop();
	bool initGL();
	bool init();
	void cleanup();
	void run();
	void performClick(S32 mouseX, S32 mouseY);
	void handleEvent(Event *event);
};

#endif
