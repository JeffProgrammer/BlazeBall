//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsEngine.h"

PhysicsEngine *PhysicsEngine::gEngine = NULL;

PhysicsEngine *PhysicsEngine::getEngine() {
	return gEngine;
}

void PhysicsEngine::setEngine(PhysicsEngine *engine) {
	gEngine = engine;

	gEngine->init();
}