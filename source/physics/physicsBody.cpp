//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsBody.h"

glm::vec3 PhysicsBody::getPosition() const {
	return btConvert(mActor->getWorldTransform().getOrigin());
}

glm::quat PhysicsBody::getRotation() const {
	return btConvert(mActor->getWorldTransform().getRotation());
}

glm::vec3 PhysicsBody::getScale() const {
	return btConvert(mActor->getCollisionShape()->getLocalScaling());
}

void PhysicsBody::setPosition(const glm::vec3 &position) {
	btTransform worldTrans = mActor->getWorldTransform();
	worldTrans.setOrigin(btConvert(position));
	mActor->setWorldTransform(worldTrans);
}

void PhysicsBody::setRotation(const glm::quat &rotation) {
	btTransform worldTrans = mActor->getWorldTransform();
	worldTrans.setRotation(btConvert(rotation));
	mActor->setWorldTransform(worldTrans);
}

void PhysicsBody::setScale(const glm::vec3 &scale) {
	mActor->getCollisionShape()->setLocalScaling(btConvert(scale));
}

void PhysicsBody::setActivationState(bool state) {
	mActor->setActivationState(state ? ACTIVE_TAG : DISABLE_SIMULATION);
}

bool PhysicsBody::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	return true;
}

void PhysicsBody::notifyContact(ContactCallbackInfo &info, bool isBody0) {
}
