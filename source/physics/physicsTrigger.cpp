//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "physics/physicsEngine.h"
#include "physics/physicsTrigger.h"
#include "game/trigger.h"

// using ghost objects in bullet:
// http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=7468

PhysicsTrigger::PhysicsTrigger(Trigger *trigger) {
	btBoxShape *shape = new btBoxShape(btConvert(trigger->getScale()));
	shape->setMargin(0.01f);
	shape->setUserPointer(trigger);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btConvert(trigger->getPosition()));
	transform.setRotation(btConvert(trigger->getRotation()));

	mActor = new btGhostObject();
	mActor->setCollisionShape(shape);
	mActor->setWorldTransform(transform);

	// Set the trigger flag on the actor.
	mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}