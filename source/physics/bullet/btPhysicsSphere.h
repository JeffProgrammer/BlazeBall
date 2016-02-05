//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_BULLET_BTPHYSICSSPHERE_H_
#define _GAME_PHYSICS_BULLET_BTPHYSICSSPHERE_H_

#include "physics/bullet/btPhysicsBody.h"
#include "physics/physicsSphere.h"

class btPhysicsSphere : public btPhysicsBody, public PhysicsSphere {
public:
	btPhysicsSphere(const F32 &mRadius);

	virtual bool getColliding();
	virtual glm::vec3 getCollisionNormal(glm::vec3 &toiVelocity);

	virtual F32 getRadius();
	virtual void setRadius(const F32 &radius);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0);
	virtual void notifyContact(ContactCallbackInfo &info, bool isBody0);
};

#endif // _GAME_PHYSICS_BULLET_BTPHYSICSSPHERE_H_