//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef scene_h
#define scene_h

#include <stdio.h>
#include "base/math.h"
#include "base/types.h"
#include "base/io.h"
#include <dif/objects/dif.h>
#include "texture/texture.h"
#include "game/sphere.h"
#include "physics/physicsEngine.h"
#include "render/shader.h"
#include "render/scene.h"
#include "platform/window.h"
#include "platform/event.h"
#include "platform/timer.h"
#include <vector>
#include "game/movement.h"
#include "game/camera.h"
#include "game/skybox.h"
#include "texture/cubeMapFramebufferTexture.h"
#include "render/modelManager.h"
#include "renderInfo.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

class Scene {
protected:
	bool running;
	bool printFPS = true;

	glm::mat4 mScreenProjectionMatrix;

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

	F32 pixelDensity;

public:
	const glm::vec4 lightColor     = glm::vec4(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const glm::vec4 ambientColor   = glm::vec4(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const glm::vec3 sunPosition    = glm::vec3(100.0f, 75.0f, 100.0f);
	const U32 specularExponent   = 7;

	std::vector<GameObject *>objects;
	std::vector<RenderedObject *> mRenderedObjects;
	GameObject *controlObject;
	Camera *mCamera;
	Sphere *mPlayer;
	Skybox *mSkybox;
	CubeMapFramebufferTexture *marbleCubemap;

	Window *window;
	Timer *mTimer;

	Shader *mInteriorShader;
	Shader *mShapeShader;
	Shader *mSkyboxShader;
	
	bool mShouldSleep;

	void addObject(GameObject *object) {
		objects.push_back(object);
		if (dynamic_cast<RenderedObject*>(object))
			mRenderedObjects.push_back(static_cast<RenderedObject*>(object));
	}

	Scene();
	~Scene();

	static Scene *getSingleton() {
		static Scene *singleton;
		if (!singleton)
			singleton = new Scene();
		return singleton;
	}

	void updateWindowSize(const glm::ivec2 &size);

	void renderScene(const RenderInfo &info);
	void render();

	void loop(const F64 &deltaMS);
	void tick(const F64 &deltaMS);
	bool initGL();
	bool init();
	void cleanup();
	void run();
	void performClick(S32 mouseX, S32 mouseY);
	void handleEvent(Event *event);

	void createCamera(const glm::vec3 &position);
	void createPlayer(const glm::vec3 &position, F32 radius);
	void createInterior(const std::string &path);
};

#endif
