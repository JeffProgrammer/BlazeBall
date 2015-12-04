//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef physicsSphere_h
#define physicsSphere_h

#include "base/types.h"

class PhysicsSphere {
public:
	virtual bool getColliding() = 0;
	virtual glm::vec3 getCollisionNormal(glm::vec3 &toiVelocity) = 0;

	virtual F32 getRadius() = 0;
	virtual void setRadius(const F32 &radius) = 0;
};

#endif
