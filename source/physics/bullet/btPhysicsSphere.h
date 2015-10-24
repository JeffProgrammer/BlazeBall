//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef btPhysicsSphere_h
#define btPhysicsSphere_h

#include "physics/bullet/btPhysicsBody.h"
#include "physics/physicsSphere.h"

class btPhysicsSphere : public btPhysicsBody, public PhysicsSphere {
	F32 mRadius;
public:
	btPhysicsSphere(const F32 &mRadius);

	virtual bool getColliding();
	virtual glm::vec3 getCollisionNormal();

	virtual F32 getRadius();
	virtual void setRadius(const F32 &radius);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0);
};

#endif /* btPhysicsSphere_h */