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

#ifndef _GAME_PHYSICS_PHYSICSBODY_H_
#define _GAME_PHYSICS_PHYSICSBODY_H_

#include "base/types.h"
#include "game/world.h"
#include "physics/physicsEngine.h"

class PhysicsBody {
protected:
	World *mWorld;
	btCollisionObject *mActor;
public:
	virtual ~PhysicsBody() {}

	virtual void setWorld(World *world) { mWorld = world; }
	virtual World *getWorld() { return mWorld; }
	
	virtual glm::vec3 getPosition() const;
	virtual glm::quat getRotation() const;
	virtual glm::vec3 getScale() const;
	virtual BoxF      getWorldBox() const;

	virtual void setPosition(const glm::vec3 &position);
	virtual void setRotation(const glm::quat &rotation);
	virtual void setScale(const glm::vec3 &scale);

	virtual void setActivationState(bool state);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0);
	virtual void notifyContact(ContactCallbackInfo &info, bool isBody0);

	btCollisionObject *getCollisionObject() {
		return mActor;
	}
};

#endif // _GAME_PHYSICS_PHYSICSBODY_H_
