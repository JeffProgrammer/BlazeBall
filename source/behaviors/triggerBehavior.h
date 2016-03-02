//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BEHAVIORS_TRIGGERBEHAVIOR_H_
#define _BEHAVIORS_TRIGGERBEHAVIOR_H_

#include "behaviors/behavior.h"
#include "behaviors/behaviorConcreteClassRep.h"

class GameObject;

/**
 * A class that represents the basis for all behaviors that have
 * trigger callbacks.
 * Please derive from this class. Do not use this as your behavior.
 */
class TriggerBehavior : public Behavior {
public:
	TriggerBehavior() {
		mTick = 100;
	}

	virtual void onEnterTrigger(GameObject *obj) {}

	virtual void onLeaveTrigger(GameObject *obj) {}

protected:
	F32 mTick;
};

#endif // _BEHAVIORS_TRIGGERBEHAVIOR_H_