//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
    virtual void setLinearVelocity(const glm::vec3 &velocity);
    virtual void setAngularVelocity(const glm::vec3 &velocity);

	virtual void applyTorque(const glm::vec3 &torque);
	virtual void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin);
	virtual void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0);

	btRigidBody *getActor() {
		return mActor;
	}
};

#endif /* btPhysicsBody_h */