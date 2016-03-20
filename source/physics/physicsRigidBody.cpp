//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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