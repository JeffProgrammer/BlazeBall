//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/bullet/btPhysicsBody.h"

F32 btPhysicsBody::getMass() const {
	//Edge case: static objects have a mass of 0
	if (mActor->getInvMass() == 0)
		return 0;
	//1 / Inverse mass because logic
	return 1.0f / mActor->getInvMass();
}
glm::vec3 btPhysicsBody::getPosition() const {
	return btConvert(mActor->getWorldTransform().getOrigin());
}
glm::quat btPhysicsBody::getRotation() const {
	return btConvert(mActor->getWorldTransform().getRotation());
}
glm::vec3 btPhysicsBody::getScale() const {
	return btConvert(mActor->getCollisionShape()->getLocalScaling());
}
glm::vec3 btPhysicsBody::getLinearVelocity() const {
	return btConvert(mActor->getLinearVelocity());
}
glm::vec3 btPhysicsBody::getAngularVelocity() const {
	return btConvert(mActor->getAngularVelocity());
}
glm::vec3 btPhysicsBody::getForce() const {
	return btConvert(mActor->getTotalForce());
}
glm::vec3 btPhysicsBody::getTorque() const {
	return btConvert(mActor->getTotalTorque());
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
void btPhysicsBody::setLinearVelocity(const glm::vec3 &velocity) {
	mActor->setLinearVelocity(btConvert(velocity));
}
void btPhysicsBody::setAngularVelocity(const glm::vec3 &velocity) {
	mActor->setAngularVelocity(btConvert(velocity));
}
void btPhysicsBody::setForce(const glm::vec3 &force) {
	mActor->setTotalForce(btConvert(force));
}
void btPhysicsBody::setTorque(const glm::vec3 &torque) {
	mActor->setTotalTorque(btConvert(torque));
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

bool btPhysicsBody::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	return true;
}

void btPhysicsBody::notifyContact(ContactCallbackInfo &info, bool isBody0) {
}
