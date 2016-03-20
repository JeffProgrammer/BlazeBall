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

#ifndef _GAME_PHYSICS_PHYSICSRIGIDBODY_H_
#define _GAME_PHYSICS_PHYSICSRIGIDBODY_H_

#include "physics/physicsBody.h"

class PhysicsRigidBody : public PhysicsBody {
public:
	virtual F32 getMass() const;
	virtual glm::vec3 getLinearVelocity() const;
	virtual glm::vec3 getAngularVelocity() const;
	virtual glm::vec3 getForce() const;
	virtual glm::vec3 getTorque() const;

	virtual void setMass(const F32 &mass);
	virtual void setLinearVelocity(const glm::vec3 &velocity);
	virtual void setAngularVelocity(const glm::vec3 &velocity);
	virtual void setForce(const glm::vec3 &force);
	virtual void setTorque(const glm::vec3 &torque);

	virtual void applyTorque(const glm::vec3 &torque);
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin);
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	inline btRigidBody* getRigidActor() const { return static_cast<btRigidBody*>(mActor); }
};

#endif // _GAME_PHYSICS_PHYSICSRIGIDBODY_H_
