//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
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

#ifndef btPhysicsBody_h
#define btPhysicsBody_h

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/physicsBody.h"

class btPhysicsBody : public PhysicsBody {
protected:
	btRigidBody *mActor;
public:
	btPhysicsBody() : mActor(nullptr) {};
	btPhysicsBody(btRigidBody *actor) : mActor(actor) {};

	virtual const F32 getMass();
	virtual const glm::vec3 getPosition();
	virtual const glm::quat getRotation();
	virtual const glm::vec3 getScale();
	virtual const glm::vec3 getLinearVelocity();
	virtual const glm::vec3 getAngularVelocity();

	virtual void setMass(const F32 &mass);
	virtual void setPosition(const glm::vec3 &position);
	virtual void setRotation(const glm::quat &rotation);
	virtual void setScale(const glm::vec3 &scale);
    virtual void setVelocity(const glm::vec3 &velocity);
    virtual void setAngularVelocity(const glm::vec3 &velocity);

	virtual void applyTorque(const glm::vec3 &torque);
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin);
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	virtual void modifyContact(ContactCallbackInfo &info, bool isBody0);

	btRigidBody *getActor() {
		return mActor;
	}
};

#endif /* btPhysicsBody_h */