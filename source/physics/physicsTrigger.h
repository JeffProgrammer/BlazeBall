//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_PHYSICSTRIGGER_H_
#define _GAME_PHYSICS_PHYSICSTRIGGER_H_

#include "physics/physicsBody.h"

class Trigger;

class PhysicsTrigger : public PhysicsBody {
public:
	PhysicsTrigger(Trigger *trigger);
	virtual ~PhysicsTrigger();

	virtual void notifyContact(ContactCallbackInfo &info, bool isBody0) override;

protected:
	Trigger *mTrigger;
};

#endif // _GAME_PHYSICS_PHYSICSTRIGGER_H_