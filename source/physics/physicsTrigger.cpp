//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "physics/physicsEngine.h"
#include "physics/physicsTrigger.h"
#include "physics/physicsSphere.h"
#include "game/sphere.h"
#include "game/trigger.h"

PhysicsTrigger::PhysicsTrigger(Trigger *trigger) {
	// We use the radius of the scale for the trigger since it takes half extents.
	auto shape = new btBoxShape(btConvert(trigger->getScale() * 0.5f));
	shape->setMargin(0.01f);

	// Set the physic's object transformation matrix to be the same
	// as the transform matrix of the game object.
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btConvert(trigger->getPosition()));
	transform.setRotation(btConvert(trigger->getRotation()));

	auto state = new btDefaultMotionState();
	state->setWorldTransform(transform);
	mActor = new btRigidBody(0.0f, state, shape);

	// Set the trigger flag on the actor.
	// CF_NO_CONTACT_RESPONSE set's it to be a trigger interaction
	// CF_CUSTOM_MATERIAL_CALLBACK let's us get notifications.
	mActor->setCollisionFlags(
		mActor->getCollisionFlags() | 
		btCollisionObject::CF_NO_CONTACT_RESPONSE | 
		btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK
	);

	// create a relationship between the trigger and the physics representation
	mTrigger = trigger;
}

PhysicsTrigger::~PhysicsTrigger() {
	delete mActor;
}

void PhysicsTrigger::notifyContact(ContactCallbackInfo &info, bool isBody0) {
	// Note: if RTII becomes to slow here, we can implement a custom integer
	// based solution so that it is blazingly fast.
	auto physTrigger = dynamic_cast<PhysicsTrigger*>(isBody0 ? info.body0 : info.body1);
	if (physTrigger == nullptr)
		return;

	// check to make sure that the sphere is the other body
	// WARNING: this could be unsafe if we use other dynamic shapes.
	// may have to switch to RTII if we do that.
	auto physSphere = static_cast<PhysicsSphere*>(isBody0 ? info.body1 : info.body0);
	
	// issue trigger event so that we can check if we've entered the trigger,
	// and act upon it.
	physTrigger->mTrigger->onEnterTrigger(physSphere->mSphere);
}