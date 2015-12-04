//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
	virtual void setLinearVelocity(const glm::vec3 &velocity) = 0;
	virtual void setAngularVelocity(const glm::vec3 &velocity) = 0;

	virtual void applyTorque(const glm::vec3 &torque) = 0;
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) = 0;
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin) = 0;

};

#endif
