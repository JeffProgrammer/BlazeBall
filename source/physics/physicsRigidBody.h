//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_PHYSICSRIGIDBODY_H_
#define _GAME_PHYSICS_PHYSICSRIGIDBODY_H_

#include "physics/physicsBody.h"

class PhysicsRigidBody : public PhysicsBody {
protected:
	btRigidBody *mActor;
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

	btRigidBody *getActor() {
		return mActor;
	}
};

#endif // _GAME_PHYSICS_PHYSICSRIGIDBODY_H_
