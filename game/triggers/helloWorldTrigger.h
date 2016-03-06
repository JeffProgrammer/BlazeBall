//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAMELOGIC_TRIGGERS_HELLOWORLDTRIGGER_H_
#define _GAMELOGIC_TRIGGERS_HELLOWORLDTRIGGER_H_

#include "behaviors/behaviorAPI.h"
#include "game/trigger.h"

/**
 * A class that represents the basis for all behaviors that have
 * trigger callbacks.
 * Please derive from this class. Do not use this as your behavior.
 */
class HelloWorldTrigger : public Behavior {
public:
	HelloWorldTrigger();

	/**
	 * Called when the behavior is added to the object.
	 * @param GameObject the reference to the object that this behavior is
	 * attached too. dynamic cast and store this to your own reference
	 * pointer on the behavior.
	 * @note Do NOT delete this reference inside the behavior.
	 */
	virtual void start(ScriptObject *gameObject) override;

	virtual void onEnterTrigger(GameObject *obj) override;

	virtual void onLeaveTrigger(GameObject *obj) override;

	DECLARE_BEHAVIOR(HelloWorldTrigger);

protected:
	std::string mHelloMessage;
	std::string mGoodbyeMessage;

	Trigger *mTrigger;
};

#endif // _BEHAVIORS_TRIGGERBEHAVIOR_H_