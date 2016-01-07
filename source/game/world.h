//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_

#include <vector>

#include "base/types.h"
#include "game/gameObject.h"
#include "physics/physicsEngine.h"

class World {
	std::vector<GameObject *> mObjects;
	PhysicsEngine *mPhysicsEngine;

public:
	World(PhysicsEngine *physics) : mPhysicsEngine(physics) {
		mPhysicsEngine->init();
	}
	virtual ~World() {
		delete mPhysicsEngine;
	}

	void addObject(GameObject *object) {
		mObjects.push_back(object);
	}

	void tick(const F64 &delta);
};

#endif
