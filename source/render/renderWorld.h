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

#include "platform/platformGL.h"
#include "base/math.h"
#include "base/types.h"
#include "base/io.h"
#include "texture/texture.h"
#include "game/sphere.h"
#include "physics/physicsEngine.h"
#include "render/shader.h"
#include "platform/platformWindow.h"
#include "platform/platformEvent.h"
#include "game/movement.h"
#include "game/camera.h"
#include "game/skybox.h"
#include "texture/cubeMapFramebufferTexture.h"
#include "render/modelManager.h"
#include "renderInfo.h"
#include "base/config.h"
#include "game/world.h"
#include "gui/renderInterface.h"
#include "gui/systemInterface.h"


class Client;

class RenderWorld : public World {
protected:
	Client *mClient;

public:
	const glm::vec4 lightColor     = glm::vec4(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const glm::vec4 ambientColor   = glm::vec4(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const glm::vec3 sunPosition    = glm::vec3(100.0f, 75.0f, 100.0f);
	const U32 specularExponent   = 7;

	std::vector<RenderedObject *> mRenderedObjects;
	Skybox *mSkybox;
	CubeMapFramebufferTexture *mMarbleCubemap;

	Shader *mShapeShader;
	
	virtual void addObject(GameObject *object);

	RenderWorld(PhysicsEngine *physics, ScriptEngine *script);
	virtual ~RenderWorld();

	void renderScene(RenderInfo &info);
	void render(RenderInfo &info);

	virtual void loop(const F64 &delta);
	virtual void tick(const F64 &delta);

	void createCamera(const glm::vec3 &position);
	void createPlayer(const glm::vec3 &position, F32 radius);
	void createInterior(const std::string &path);

	Client *getClient() const { return mClient; }
	void setClient(Client *client) { mClient = client; }
};

#endif
