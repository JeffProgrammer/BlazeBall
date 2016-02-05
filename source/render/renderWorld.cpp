//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/renderWorld.h"
#include "game/gameInterior.h"
#include "game/camera.h"
//#include "game/shape.h"
#include "game/skybox.h"
#include "render/util.h"
#include "network/client.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>
#include <glm/gtx/vector_angle.hpp>

glm::mat4 RenderInfo::inverseRotMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

/// The amount of time that has to pass before a tick happens.
/// Default is 16.6667 ms which means we tick at 60 frames per second
#define TICK_MS 16.6666666666666667

RenderWorld::RenderWorld(PhysicsEngine *physics, ScriptEngine *script) : World(physics, script) {
	mMarbleCubemap = nullptr;
	mDoDebugDraw = false;
}

RenderWorld::~RenderWorld() {
}

void RenderWorld::render(RenderInfo &info) {
	//TODO: Have this be on the marble somewhere, because this is just gross
	if (mMarbleCubemap == nullptr) {
		mMarbleCubemap = new CubeMapFramebufferTexture(glm::ivec2(64));
	}
	
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
	info.sunPosition = sunPosition;
	info.specularExponent = specularExponent;

	info.isReflectionPass = false;
	info.renderWorld = RenderInfo::RenderWorldMethod::from_method<RenderWorld, &RenderWorld::renderScene>(this);

	glm::vec3 position = (mClient->getControlObject() ? mClient->getControlObject()->getPosition() : glm::vec3(0));
	mMarbleCubemap->generateBuffer(position, info);

	//Actually render everything
	renderScene(info);

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

	//TODO: Clean up shape rendering
//	mShapeShader->activate();
//	info.loadShader(mShapeShader);
//
//	MODELMGR->render(mShapeShader, info.viewMatrix, info.projectionMatrix);
//	mShapeShader->deactivate();

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
