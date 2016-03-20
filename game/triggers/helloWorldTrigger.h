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