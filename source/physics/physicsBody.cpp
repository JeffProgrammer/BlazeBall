//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsBody.h"

F32 PhysicsBody::getMass() const {
	//Edge case: static objects have a mass of 0
	if (mActor->getInvMass() == 0)
		return 0;
	//1 / Inverse mass because logic
	return 1.0f / mActor->getInvMass();
}

glm::vec3 PhysicsBody::getPosition() const {
	return btConvert(mActor->getWorldTransform().getOrigin());
}

glm::quat PhysicsBody::getRotation() const {
	return btConvert(mActor->getWorldTransform().getRotation());
}

glm::vec3 PhysicsBody::getScale() const {
	return btConvert(mActor->getCollisionShape()->getLocalScaling());
}

glm::vec3 PhysicsBody::getLinearVelocity() const {
	return btConvert(mActor->getLinearVelocity());
}

glm::vec3 PhysicsBody::getAngularVelocity() const {
	return btConvert(mActor->getAngularVelocity());
}

glm::vec3 PhysicsBody::getForce() const {
	return btConvert(mActor->getTotalForce());
}

glm::vec3 PhysicsBody::getTorque() const {
	return btConvert(mActor->getTotalTorque());
}

void PhysicsBody::setMass(const F32 &mass) {
	btVector3 inertia;
	mActor->getCollisionShape()->calculateLocalInertia(mass, inertia);

	mActor->setMassProps(mass, inertia);
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

void PhysicsBody::setLinearVelocity(const glm::vec3 &velocity) {
	mActor->setLinearVelocity(btConvert(velocity));
}

void PhysicsBody::setAngularVelocity(const glm::vec3 &velocity) {
	mActor->setAngularVelocity(btConvert(velocity));
}

void PhysicsBody::setForce(const glm::vec3 &force) {
	mActor->setTotalForce(btConvert(force));
}

void PhysicsBody::setTorque(const glm::vec3 &torque) {
	mActor->setTotalTorque(btConvert(torque));
}

void PhysicsBody::setActivationState(bool state) {
	mActor->setActivationState(state ? ACTIVE_TAG : DISABLE_SIMULATION);
}

void PhysicsBody::applyTorque(const glm::vec3 &torque) {
	mActor->applyTorque(btConvert(torque));
}

void PhysicsBody::applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) {
	mActor->applyImpulse(btConvert(impulse), btConvert(origin));
}

void PhysicsBody::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyForce(btConvert(force), btConvert(origin));
}

bool PhysicsBody::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	return true;
}

void PhysicsBody::notifyContact(ContactCallbackInfo &info, bool isBody0) {
}
