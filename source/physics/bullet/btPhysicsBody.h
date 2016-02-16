//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_BULLET_BTPHYSICSBODY_H_
#define _GAME_PHYSICS_BULLET_BTPHYSICSBODY_H_

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/physicsBody.h"

class btPhysicsBody : public PhysicsBody {
protected:
	btRigidBody *mActor;
public:
	btPhysicsBody() : mActor(nullptr) {};
	btPhysicsBody(btRigidBody *actor) : mActor(actor) {};

	virtual F32 getMass() const;
	virtual glm::vec3 getPosition() const;
	virtual glm::quat getRotation() const;
	virtual glm::vec3 getScale() const;
	virtual glm::vec3 getLinearVelocity() const;
	virtual glm::vec3 getAngularVelocity() const;
	virtual glm::vec3 getForce() const;
	virtual glm::vec3 getTorque() const;

	virtual void setMass(const F32 &mass);
	virtual void setPosition(const glm::vec3 &position);
	virtual void setRotation(const glm::quat &rotation);
	virtual void setScale(const glm::vec3 &scale);
    virtual void setLinearVelocity(const glm::vec3 &velocity);
    virtual void setAngularVelocity(const glm::vec3 &velocity);
	virtual void setForce(const glm::vec3 &force);
	virtual void setTorque(const glm::vec3 &torque);

	virtual void setActivationState(bool state);

	virtual void applyTorque(const glm::vec3 &torque);
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin);
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0);
	virtual void notifyContact(ContactCallbackInfo &info, bool isBody0);

	btRigidBody *getActor() {
		return mActor;
	}
};

#endif // _GAME_PHYSICS_BULLET_BTPHYSICSBODY_H_