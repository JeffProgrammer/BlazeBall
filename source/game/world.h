//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	std::vector<ScriptObject *> mObjects;
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

	const std::vector<ScriptObject *> &getObjectList() { return mObjects; }
	GameObject *findGameObject(const std::string &name);
	virtual void addObject(GameObject *object);

	static void initFields();
};

#endif // _GAME_WORLD_H_
