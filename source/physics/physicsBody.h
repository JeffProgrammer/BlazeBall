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

#ifndef physicsBody_h
#define physicsBody_h

#include "base/types.h"
#include "physics/physicsEngine.h"

class PhysicsBody {
public:
	virtual const F32 getMass() = 0;
	virtual const glm::vec3 getPosition() = 0;
	virtual const glm::quat getRotation() = 0;
	virtual const glm::vec3 getScale() = 0;
	virtual const glm::vec3 getLinearVelocity() = 0;
	virtual const glm::vec3 getAngularVelocity() = 0;

	virtual void setMass(const F32 &mass) = 0;
	virtual void setPosition(const glm::vec3 &position) = 0;
	virtual void setRotation(const glm::quat &rotation) = 0;
	virtual void setScale(const glm::vec3 &scale) = 0;
    virtual void setVelocity(const glm::vec3 &velocity) = 0;
    virtual void setAngularVelocity(const glm::vec3 &velocity) = 0;

	virtual void applyTorque(const glm::vec3 &torque) = 0;
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) = 0;
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin) = 0;

};

#endif
