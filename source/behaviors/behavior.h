//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BEHAVIORS_BEHAVIOR_H_
#define _BEHAVIORS_BEHAVIOR_H_

#include "base/types.h"

class ScriptObject;
class GameObject;

/**
 * An abstract class that represents the basis for all behaviors.
 */
class Behavior {
public:
	/**
	 * Called when the behavior is added to the object.
	 * @param GameObject the reference to the object that this behavior is
	 * attached too. dynamic cast and store this to your own reference
	 * pointer on the behavior.
	 * @note Do NOT delete this reference inside the behavior.
	 */
	virtual void start(ScriptObject *gameObject) = 0;

	/**
	 * Called when the behavior is deleted on the object.
	 * Note that this will always be called before the object is deleted,
	 * so it is safe to reference the object in this method.
	 */
	virtual void cleanup() {}

	/**
	 * Called when the game ticks every frame.
	 * @param dt The delta time between updates.
	 */
	virtual void update(const F64 &dt) {}

	/**
	 * Called when a game object enters the trigger.
	 * @param obj The object that collided with the trigger.
	 * @Note This callback will only be fired if the behavior is attached to an
	 *  object of type Trigger.
	 */
	virtual void onEnterTrigger(GameObject *obj) {}

	/**
	 * Called when a game object leaves the trigger.
	 * @param obj The object that collided with the trigger.
	 * @Note This callback will only be fired if the behavior is attached to an
	 *  object of type Trigger.
	 */
	virtual void onLeaveTrigger(GameObject *obj) {}
};

#endif // _BEHAVIORS_BEHAVIOR_H_