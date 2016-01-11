//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef physicsBody_h
#define physicsBody_h

#include "base/types.h"
#include "game/world.h"
#include "physics/physicsEngine.h"

class PhysicsBody {
	World *mWorld;
public:
	virtual void setWorld(World *world) { mWorld = world; }
	virtual World *getWorld() { return mWorld; }
	
	virtual F32 getMass() const = 0;
	virtual glm::vec3 getPosition() const = 0;
	virtual glm::quat getRotation() const = 0;
	virtual glm::vec3 getScale() const = 0;
	virtual glm::vec3 getLinearVelocity() const = 0;
	virtual glm::vec3 getAngularVelocity() const = 0;
	virtual glm::vec3 getForce() const = 0;
	virtual glm::vec3 getTorque() const = 0;

	virtual void setMass(const F32 &mass) = 0;
	virtual void setPosition(const glm::vec3 &position) = 0;
	virtual void setRotation(const glm::quat &rotation) = 0;
	virtual void setScale(const glm::vec3 &scale) = 0;
	virtual void setLinearVelocity(const glm::vec3 &velocity) = 0;
	virtual void setAngularVelocity(const glm::vec3 &velocity) = 0;
	virtual void setForce(const glm::vec3 &force) = 0;
	virtual void setTorque(const glm::vec3 &torque) = 0;

	virtual void setActivationState(bool state) = 0;

	virtual void applyTorque(const glm::vec3 &torque) = 0;
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) = 0;
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin) = 0;
};

#endif
