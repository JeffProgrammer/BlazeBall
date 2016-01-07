//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "world.h"

void World::tick(const F64 &delta) {
	mPhysicsEngine->simulate(delta);
}
