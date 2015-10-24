//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/bullet/btPhysicsBody.h"

const F32 btPhysicsBody::getMass() {
	//Edge case: static objects have a mass of 0
	if (mActor->getInvMass() == 0)
		return 0;
	//1 / Inverse mass because logic
	return 1.0f / mActor->getInvMass();
}
const glm::vec3 btPhysicsBody::getPosition() {
	return btConvert(mActor->getWorldTransform().getOrigin());
}
const glm::quat btPhysicsBody::getRotation() {
	return btConvert(mActor->getWorldTransform().getRotation());
}
const glm::vec3 btPhysicsBody::getScale() {
	return btConvert(mActor->getCollisionShape()->getLocalScaling());
}
const glm::vec3 btPhysicsBody::getLinearVelocity() {
	return btConvert(mActor->getLinearVelocity());
}
const glm::vec3 btPhysicsBody::getAngularVelocity() {
	return btConvert(mActor->getAngularVelocity());
}

void btPhysicsBody::setMass(const F32 &mass) {
	btVector3 inertia;
	mActor->getCollisionShape()->calculateLocalInertia(mass, inertia);

	mActor->setMassProps(mass, inertia);
}
void btPhysicsBody::setPosition(const glm::vec3 &position) {
	btTransform worldTrans = mActor->getWorldTransform();
	worldTrans.setOrigin(btConvert(position));
	mActor->setWorldTransform(worldTrans);
}
void btPhysicsBody::setRotation(const glm::quat &rotation) {
	btTransform worldTrans = mActor->getWorldTransform();
	worldTrans.setRotation(btConvert(rotation));
	mActor->setWorldTransform(worldTrans);
}
void btPhysicsBody::setScale(const glm::vec3 &scale) {
	mActor->getCollisionShape()->setLocalScaling(btConvert(scale));
}

void btPhysicsBody::applyTorque(const glm::vec3 &torque) {
	mActor->applyTorque(btConvert(torque));
}
void btPhysicsBody::applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) {
	mActor->applyImpulse(btConvert(impulse), btConvert(origin));
}
void btPhysicsBody::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyForce(btConvert(force), btConvert(origin));
}

void btPhysicsBody::setVelocity(const glm::vec3 &velocity) {
    mActor->setLinearVelocity(btConvert(velocity));
}
void btPhysicsBody::setAngularVelocity(const glm::vec3 &velocity) {
    mActor->setAngularVelocity(btConvert(velocity));
}

bool btPhysicsBody::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	return true;
}
