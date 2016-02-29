//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsRigidBody.h"

F32 PhysicsRigidBody::getMass() const {
	//Edge case: static objects have a mass of 0
	if (mActor->getInvMass() == 0)
		return 0;
	//1 / Inverse mass because logic
	return 1.0f / mActor->getInvMass();
}

glm::vec3 PhysicsRigidBody::getLinearVelocity() const {
	return btConvert(mActor->getLinearVelocity());
}

glm::vec3 PhysicsRigidBody::getAngularVelocity() const {
	return btConvert(mActor->getAngularVelocity());
}

glm::vec3 PhysicsRigidBody::getForce() const {
	return btConvert(mActor->getTotalForce());
}

glm::vec3 PhysicsRigidBody::getTorque() const {
	return btConvert(mActor->getTotalTorque());
}

void PhysicsRigidBody::setMass(const F32 &mass) {
	btVector3 inertia;
	mActor->getCollisionShape()->calculateLocalInertia(mass, inertia);

	mActor->setMassProps(mass, inertia);
}

void PhysicsRigidBody::setLinearVelocity(const glm::vec3 &velocity) {
	mActor->setLinearVelocity(btConvert(velocity));
}

void PhysicsRigidBody::setAngularVelocity(const glm::vec3 &velocity) {
	mActor->setAngularVelocity(btConvert(velocity));
}

void PhysicsRigidBody::setForce(const glm::vec3 &force) {
	mActor->setTotalForce(btConvert(force));
}

void PhysicsRigidBody::setTorque(const glm::vec3 &torque) {
	mActor->setTotalTorque(btConvert(torque));
}

void PhysicsRigidBody::applyTorque(const glm::vec3 &torque) {
	mActor->applyTorque(btConvert(torque));
}

void PhysicsRigidBody::applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) {
	mActor->applyImpulse(btConvert(impulse), btConvert(origin));
}

void PhysicsRigidBody::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyForce(btConvert(force), btConvert(origin));
}