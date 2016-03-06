//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_PHYSICSSPHERE_H_
#define _GAME_PHYSICS_PHYSICSSPHERE_H_

#include "base/types.h"
#include "physics/physicsRigidBody.h"

class Sphere;

class PhysicsSphere : public PhysicsRigidBody {
	friend class PhysicsTrigger;
public:
	PhysicsSphere(Sphere *sphere, const F32 &radius);

	virtual bool getColliding();
	virtual glm::vec3 getCollisionNormal(glm::vec3 &toiVelocity);

	F32 getRadius();
	void setRadius(const F32 &radius);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0) override;
	virtual void notifyContact(ContactCallbackInfo &info, bool isBody0) override;

protected:
	Sphere *mSphere;
};

#endif // _GAME_PHYSICS_PHYSICSSPHERE_H_
