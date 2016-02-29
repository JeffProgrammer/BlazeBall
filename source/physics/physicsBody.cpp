//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsBody.h"

glm::vec3 PhysicsBody::getPosition() const {
	return btConvert(mCollision->getWorldTransform().getOrigin());
}

glm::quat PhysicsBody::getRotation() const {
	return btConvert(mCollision->getWorldTransform().getRotation());
}

glm::vec3 PhysicsBody::getScale() const {
	return btConvert(mCollision->getCollisionShape()->getLocalScaling());
}

void PhysicsBody::setPosition(const glm::vec3 &position) {
	btTransform worldTrans = mCollision->getWorldTransform();
	worldTrans.setOrigin(btConvert(position));
	mCollision->setWorldTransform(worldTrans);
}

void PhysicsBody::setRotation(const glm::quat &rotation) {
	btTransform worldTrans = mCollision->getWorldTransform();
	worldTrans.setRotation(btConvert(rotation));
	mCollision->setWorldTransform(worldTrans);
}

void PhysicsBody::setScale(const glm::vec3 &scale) {
	mCollision->getCollisionShape()->setLocalScaling(btConvert(scale));
}

void PhysicsBody::setActivationState(bool state) {
	mCollision->setActivationState(state ? ACTIVE_TAG : DISABLE_SIMULATION);
}

bool PhysicsBody::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	return true;
}

void PhysicsBody::notifyContact(ContactCallbackInfo &info, bool isBody0) {
}
