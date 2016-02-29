//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "physics/physicsEngine.h"
#include "physics/physicsTrigger.h"
#include "game/trigger.h"

PhysicsTrigger::PhysicsTrigger(Trigger *trigger) {
	auto shape = new btBoxShape(btConvert(trigger->getScale()));
	shape->setMargin(0.01f);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btConvert(trigger->getPosition()));
	transform.setRotation(btConvert(trigger->getRotation()));

	auto state = new btDefaultMotionState();
	state->setWorldTransform(transform);
	mActor = new btRigidBody(0.0f, state, shape);

	// Set the trigger flag on the actor.
	mActor->setCollisionFlags(
		mActor->getCollisionFlags() | 
		btCollisionObject::CF_NO_CONTACT_RESPONSE | 
		btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK
	);
}

void PhysicsTrigger::notifyContact(ContactCallbackInfo &info, bool isBody0) {
	IO::printf("OMG!\n");
}