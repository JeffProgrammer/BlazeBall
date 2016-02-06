//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_

#include <vector>

#include "base/types.h"
#include "physics/physicsEngine.h"
#include "game/scriptObject.h"

class GameObject;
class World : public ScriptObject {
protected:
	std::vector<GameObject *> mObjects;
	PhysicsEngine *mPhysicsEngine;

	bool mRunning;
	bool mShouldSleep;

	F32 mSimulationSpeed;

	DECLARE_SCRIPTOBJECT(World);
public:
	World() {
		//Needed to make ConcreteClassRep shut up; don't use this
		assert(false);
	}
	World(PhysicsEngine *physics);
	virtual ~World();

	PhysicsEngine *getPhysicsEngine() { return mPhysicsEngine; }

	bool getRunning() { return mRunning; }
	void setRunning(bool running) { mRunning = running; }

	virtual void tick(const F64 &delta);
	virtual void loop(const F64 &delta);
	
	bool loadLevel(const std::string &file);

	const std::vector<GameObject *> &getObjectList() { return mObjects; }
	GameObject *findGameObject(const std::string &name);
	virtual void addObject(GameObject *object);

	static void initFields();
};

#endif // _GAME_WORLD_H_
