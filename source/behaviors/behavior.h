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
	 * Called whenever the physics engine ticks.
	 * @param dt The time delta between each physics tick.
	 */
	virtual void updateTick(const F64 &dt) {}

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