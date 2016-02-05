//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
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
	bool mDoDebugDraw;

public:
	const glm::vec4 lightColor     = glm::vec4(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const glm::vec4 ambientColor   = glm::vec4(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const glm::vec3 sunPosition    = glm::vec3(100.0f, 75.0f, 100.0f);
	const U32 specularExponent   = 7;

	std::vector<RenderedObject *> mRenderedObjects;
	Skybox *mSkybox;
	Shader *mShapeShader;
	
	virtual void addObject(GameObject *object);

	RenderWorld(PhysicsEngine *physics, ScriptEngine *script);
	virtual ~RenderWorld();

	void renderScene(RenderInfo &info);
	void render(RenderInfo &info);

	Client *getClient() const { return mClient; }
	void setClient(Client *client) { mClient = client; }
};

#endif // _RENDER_RENDERWORLD_H_