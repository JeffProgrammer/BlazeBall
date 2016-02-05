//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_PHYSICSSPHERE_H_
#define _GAME_PHYSICS_PHYSICSSPHERE_H_

#include "base/types.h"

class PhysicsSphere {
public:
	virtual bool getColliding() = 0;
	virtual glm::vec3 getCollisionNormal(glm::vec3 &toiVelocity) = 0;

	virtual F32 getRadius() = 0;
	virtual void setRadius(const F32 &radius) = 0;
};

#endif // _GAME_PHYSICS_PHYSICSSPHERE_H_
