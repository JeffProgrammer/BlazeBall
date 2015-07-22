//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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
#include "bitmap/texture.h"
#include "game/sphere.h"
#include "physics/physicsEngine.h"
#include "render/shader.h"
#include "render/scene.h"
#include "platform/window.h"
#include "platform/event.h"

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
    bool printFPS = false;

	F32 maxFPS = 60.0f;

	GLfloat angle;

	F32 yaw;
	F32 pitch;
	glm::vec3 cameraPosition;

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

	GLuint displayList;
	bool listNeedsDisplay = true;

	bool captureMouse = false;
	bool mouseButtons[3] = {false, false, false};
	bool movement[9] = {false, false, false, false, false, false, false, false, false};

	bool convertMode;

	const float cameraSpeed = 0.3f;
	const float keyCameraSpeed = 3.f;
	const float movementSpeed = 0.2f;

	const ColorF lightColor      = ColorF(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const Point3F lightDirection = Point3F(0.60f, 0.40f, 1.0f);
	const ColorF ambientColor    = ColorF(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const Point3F sunPosition    = Point3F(100.0f, 75.0f, 100.0f);
	const F32 sunPower           = 1000.0f;
	const U32 specularExponent   = 7;
public:
	U32 difCount;
	DIF::DIF **difs;
	String **filenames;
	Sphere *sphere;

	Window *window;

	static Scene *getSingleton() {
		static Scene *singleton;
		if (!singleton)
			singleton = new Scene();
		return singleton;
	}
	void setLoopCallback(void (*callback)()) {
		loopCallback = callback;
	}

	bool getConvertMode() {
		return convertMode;
	}
	void setConvertMode(bool convertMode) {
		this->convertMode = convertMode;
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
