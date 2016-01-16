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
#include "game/scriptObject.h"

class GameObject;
class World : public ScriptObject {
protected:
	std::vector<GameObject *> mObjects;
	PhysicsEngine *mPhysicsEngine;
	ScriptEngine *mScriptEngine;

	bool mRunning;
	bool mShouldSleep;

	F32 mSimulationSpeed;

	DECLARE_SCRIPTOBJECT(World);
public:
	World() {
		//Needed to make ConcreteClassRep shut up; don't use this
		assert(false);
	}
	World(PhysicsEngine *physics, ScriptEngine *script);
	virtual ~World();

	PhysicsEngine *getPhysicsEngine() { return mPhysicsEngine; }
	ScriptEngine *getScriptEngine() { return mScriptEngine; }

	bool getRunning() { return mRunning; }
	void setRunning(bool running) { mRunning = running; }

	virtual void tick(const F64 &delta);
	virtual void loop(const F64 &delta);
	
	bool loadLevel(const std::string &file);

	const std::vector<GameObject *> &getObjectList() { return mObjects; }
	GameObject *findGameObject(const std::string &name);
	virtual void addObject(GameObject *object);

	/**
	 * Initializes the scripting API for the respective sript engine.
	 * @param engine The script engine to initialize to.
	 */
	static void initScript(ScriptEngine *engine);
};

#endif
