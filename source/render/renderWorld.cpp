//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/renderWorld.h"

#include <glm/gtx/vector_angle.hpp>
#include "render/util.h"
#include "network/client.h"

glm::mat4 RenderInfo::inverseRotMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

/// The amount of time that has to pass before a tick happens.
/// Default is 16.6667 ms which means we tick at 60 frames per second
#define TICK_MS 16.6666666666666667

RenderWorld::RenderWorld(PhysicsEngine *physics) : World(physics) {
	mDoDebugDraw = false;
}

RenderWorld::~RenderWorld() {
}

void RenderWorld::generateBuffers() {
	mFramebufferTexture = nullptr;

	// now, set up the front buffer, which is simply the quad in which we will render our texture in
	static const GLfloat quad[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	// create and fill buffer
	glGenBuffers(1, &mFramebufferVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mFramebufferVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mFramebufferShader = Shader::getShaderByName("PostFX");
}

void RenderWorld::render(RenderInfo &info) {
	//Get the camera transform from the marble
	glm::mat4 cameraTransform(1.0f);
	glm::vec3 cameraPosition(0.0f);

	if (mClient->getControlObject())
		mClient->getControlObject()->getCameraPosition(cameraTransform, cameraPosition);

	//Camera
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	viewMatrix *= cameraTransform;

	info.viewMatrix = viewMatrix;
	info.cameraPosition = cameraPosition;

	info.lightColor = lightColor;
	info.ambientColor = ambientColor;
	info.sunDirection = sunDirection;
	info.specularExponent = specularExponent;

	info.isReflectionPass = false;
	info.renderWorld = RenderInfo::RenderWorldMethod::from_method<RenderWorld, &RenderWorld::renderScene>(this);

	if (mFramebufferTexture == nullptr) {
		mFramebufferTexture = new FramebufferTexture(info.viewport.size * (S32)info.pixelDensity);
	}

	mFramebufferTexture->generateBuffer(info);
	GL_CHECKERRORS();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	mFramebufferShader->activate();
	mFramebufferTexture->activate(GL_TEXTURE0);

	mFramebufferShader->setUniformVector("inScreenSize", info.viewport.size);
	mFramebufferShader->setUniformVector("inProjectionBounds", glm::vec2(0.1f, 500.f));

	GL_CHECKERRORS();

	glBindBuffer(GL_ARRAY_BUFFER, mFramebufferVBO);
	GL_CHECKERRORS();

	mFramebufferShader->enableAttributes();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	mFramebufferShader->disableAttributes();
	GL_CHECKERRORS();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mFramebufferTexture->deactivate();
	mFramebufferShader->deactivate();
	GL_CHECKERRORS();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if (mDoDebugDraw) {
		glDisable(GL_DEPTH_TEST);
		mClient->getWorld()->getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Everything);
		glEnable(GL_DEPTH_TEST);
	} else {
		mClient->getWorld()->getPhysicsEngine()->debugDraw(info, PhysicsEngine::DebugDrawType::Nothing);
	}
}

void RenderWorld::renderScene(RenderInfo &info) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render all the mObjects in the scene
	for (auto object : mRenderedObjects) {
		//Don't render non-renderable mObjects (like the camera)
		//Also don't render some mObjects in reflections (like particles)
		if (object->isRenderable() && (!info.isReflectionPass || object->isReflectable())) {
			object->render(info);
		}
	}

	//Run all render methods
	info.render();
	info.clearRenderMethods();

	// check for opengl errors
	GL_CHECKERRORS();
}

void RenderWorld::addObject(GameObject *object) {
	World::addObject(object);

	// For rendered objects.
	auto *renderedObject = dynamic_cast<RenderedObject*>(object);
	if (renderedObject != nullptr) {
		mRenderedObjects.push_back(renderedObject);
	}
}
