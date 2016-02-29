//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsRigidBody.h"

F32 PhysicsRigidBody::getMass() const {
	//Edge case: static objects have a mass of 0
	if (getRigidActor()->getInvMass() == 0)
		return 0;
	//1 / Inverse mass because logic
	return 1.0f / getRigidActor()->getInvMass();
}

glm::vec3 PhysicsRigidBody::getLinearVelocity() const {
	return btConvert(getRigidActor()->getLinearVelocity());
}

glm::vec3 PhysicsRigidBody::getAngularVelocity() const {
	return btConvert(getRigidActor()->getAngularVelocity());
}

glm::vec3 PhysicsRigidBody::getForce() const {
	return btConvert(getRigidActor()->getTotalForce());
}

glm::vec3 PhysicsRigidBody::getTorque() const {
	return btConvert(getRigidActor()->getTotalTorque());
}

void PhysicsRigidBody::setMass(const F32 &mass) {
	btVector3 inertia;
	getRigidActor()->getCollisionShape()->calculateLocalInertia(mass, inertia);

	getRigidActor()->setMassProps(mass, inertia);
}

void PhysicsRigidBody::setLinearVelocity(const glm::vec3 &velocity) {
	getRigidActor()->setLinearVelocity(btConvert(velocity));
}

void PhysicsRigidBody::setAngularVelocity(const glm::vec3 &velocity) {
	getRigidActor()->setAngularVelocity(btConvert(velocity));
}

void PhysicsRigidBody::setForce(const glm::vec3 &force) {
	getRigidActor()->setTotalForce(btConvert(force));
}

void PhysicsRigidBody::setTorque(const glm::vec3 &torque) {
	getRigidActor()->setTotalTorque(btConvert(torque));
}

void PhysicsRigidBody::applyTorque(const glm::vec3 &torque) {
	getRigidActor()->applyTorque(btConvert(torque));
}

void PhysicsRigidBody::applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) {
	getRigidActor()->applyImpulse(btConvert(impulse), btConvert(origin));
}

void PhysicsRigidBody::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	getRigidActor()->applyForce(btConvert(force), btConvert(origin));
}