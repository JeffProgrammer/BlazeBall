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
#include "texture/framebufferTexture.h"
#include "physics/physicsEngine.h"
#include "render/renderInfo.h"
#include "game/renderedObject.h"
#include "game/world.h"

class Client;

class RenderWorld : public World {
protected:
	Client *mClient;
	bool mDoDebugDraw;
	std::vector<RenderedObject *> mRenderedObjects;

	Shader *mFramebufferShader;
	FramebufferTexture *mFramebufferTexture;
	GLuint mFramebufferVBO;

	bool mUsePostFX;

public:
	const glm::vec4 lightColor     = glm::vec4(1.000000f, 1.000000f, 1.000000f, 1.400000f);
	const glm::vec4 ambientColor   = glm::vec4(0.700000f, 0.700000f, 0.700000f, 1.000000f);

	const glm::vec3 sunDirection   = glm::vec3(0.57f, 0.28f, -0.77f);
	const U32 specularExponent   = 7;

	virtual void addObject(GameObject *object);
	void generateBuffers();

	RenderWorld(PhysicsEngine *physics);
	virtual ~RenderWorld();

	void renderScene(RenderInfo &info);
	void render(RenderInfo &info);

	Client *getClient() const { return mClient; }
	void setClient(Client *client) { mClient = client; }
};

#endif // _RENDER_RENDERWORLD_H_