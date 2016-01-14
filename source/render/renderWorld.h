//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_RENDERWORLD_H_
#define _RENDER_RENDERWORLD_H_

#include <stdio.h>
#include <vector>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <dif/objects/dif.h>

#include "base/math.h"
#include "base/types.h"
#include "base/io.h"
#include "texture/texture.h"
#include "game/sphere.h"
#include "physics/physicsEngine.h"
#include "render/shader.h"
#include "platform/platformWindow.h"
#include "platform/platformEvent.h"
#include "platform/platformTimer.h"
#include "game/movement.h"
#include "game/camera.h"
#include "game/skybox.h"
#include "texture/cubeMapFramebufferTexture.h"
#include "render/modelManager.h"
#include "renderInfo.h"
#include "base/config.h"
#include "game/world.h"

class Client;

class RenderWorld : public World {
protected:
	bool mPrintFPS = true;

	glm::mat4 mScreenProjectionMatrix;

	struct {
		bool hasSelection;
		U32 surfaceIndex;
		GameInterior *interior;
	} mSelection;

	bool mCaptureMouse = false;
	struct {
		bool left;
		bool middle;
		bool right;
	} mouseButtons;
	Movement mMovement;

	F32 mPixelDensity;

	bool mDoDebugDraw;
	Client *mClient;

public:
	const glm::vec4 lightColor     = glm::vec4(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const glm::vec4 ambientColor   = glm::vec4(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const glm::vec3 sunPosition    = glm::vec3(100.0f, 75.0f, 100.0f);
	const U32 specularExponent   = 7;

	std::vector<RenderedObject *> mRenderedObjects;
	Skybox *mSkybox;
	CubeMapFramebufferTexture *mMarbleCubemap;

	PlatformTimer *mTimer;
	PlatformWindow *mWindow;
	Config *mConfig;

	Shader *mShapeShader;
	
	virtual void addObject(GameObject *object);

	RenderWorld(PhysicsEngine *physics);
	virtual ~RenderWorld();

	void updateWindowSize(const glm::ivec2 &size);

	void renderScene(RenderInfo &info);
	void render();

	virtual void loop(const F64 &delta);
	virtual void tick(const F64 &delta);
	bool initGL();
	bool init();
	void performClick(S32 mouseX, S32 mouseY);
	void handleEvent(PlatformEvent *event);

	void createCamera(const glm::vec3 &position);
	void createPlayer(const glm::vec3 &position, F32 radius);
	void createInterior(const std::string &path);

	Client *getClient() const { return mClient; }
	void setClient(Client *client) { mClient = client; }
};

#endif
