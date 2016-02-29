//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_PHYSICSBODY_H_
#define _GAME_PHYSICS_PHYSICSBODY_H_

#include "base/types.h"
#include "game/world.h"
#include "physics/physicsEngine.h"

class PhysicsBody {
protected:
	World *mWorld;
	btCollisionObject *mCollision;
public:
	virtual void setWorld(World *world) { mWorld = world; }
	virtual World *getWorld() { return mWorld; }
	
	virtual glm::vec3 getPosition() const;
	virtual glm::quat getRotation() const;
	virtual glm::vec3 getScale() const;

	virtual void setPosition(const glm::vec3 &position);
	virtual void setRotation(const glm::quat &rotation);
	virtual void setScale(const glm::vec3 &scale);

	virtual void setActivationState(bool state);

	virtual bool modifyContact(ContactCallbackInfo &info, bool isBody0);
	virtual void notifyContact(ContactCallbackInfo &info, bool isBody0);

	btCollisionObject *getCollisionObject() {
		return mCollision;
	}
};

#endif // _GAME_PHYSICS_PHYSICSBODY_H_
