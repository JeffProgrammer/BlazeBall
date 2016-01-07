//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_

#include <vector>

#include "base/types.h"
#include "physics/physicsEngine.h"

class GameObject;
class World {
protected:
	std::vector<GameObject *> mObjects;
	PhysicsEngine *mPhysicsEngine;

	bool mRunning;
	bool mShouldSleep;

	F32 mSimulationSpeed;

public:
	World(PhysicsEngine *physics);
	virtual ~World();

	PhysicsEngine *getPhysicsEngine() { return mPhysicsEngine; }

	bool getRunning() { return mRunning; }
	void setRunning(bool running) { mRunning = running; }

	virtual void tick(const F64 &delta);
	virtual void loop(const F64 &delta);
	
	bool loadLevel(const std::string &file);

	GameObject *findGameObject(const std::string &name);
	virtual void addObject(GameObject *object) {
		mObjects.push_back(object);
	}
};

#endif
