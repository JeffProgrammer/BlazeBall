//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
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

#include "pxPhysicsBody.h"

const glm::vec3 PxPhysicsBody::getPosition() {
	return pxConvert(mActor->getGlobalPose().p);
}
const glm::quat PxPhysicsBody::getRotation() {
	return pxConvert(mActor->getGlobalPose());
}

void PxPhysicsBody::setMass(const F32 &mass) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->setMass(mass);
	}
}
void PxPhysicsBody::setPosition(const glm::vec3 &position) {
	physx::PxTransform trans;
	trans.p = pxConvert(position);
	trans.q = mActor->getGlobalPose().q;
	mActor->setGlobalPose(trans);
}
void PxPhysicsBody::setRotation(const glm::quat &rotation) {
	physx::PxTransform trans;
	trans.p = mActor->getGlobalPose().p;
	trans.q = pxConvert(rotation);
	mActor->setGlobalPose(trans);
}
void PxPhysicsBody::applyTorque(const glm::vec3 &torque) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->addTorque(pxConvert(torque));
	}
}
void PxPhysicsBody::applyImpulse(const glm::vec3 &impulse, const glm::vec3 &origin) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->addForce(pxConvert(impulse), physx::PxForceMode::eIMPULSE);
//		physx::PxRigidBodyExt::addForceAtLocalPos(*dyn, pxConvert(impulse), pxConvert(origin));
	}
}
void PxPhysicsBody::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->addForce(pxConvert(force));
//		physx::PxRigidBodyExt::addForceAtLocalPos(*dyn, pxConvert(force), pxConvert(origin));
	}
}

bool PxPhysicsBody::getDynamic() {
	return mActor->is<physx::PxRigidDynamic>();
}

void PxPhysicsBody::setScale(const glm::vec3 &scale) {
	//Not possible in PhysX
}
void PxPhysicsBody::setLinearVelocity(const glm::vec3 &velocity) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->setLinearVelocity(pxConvert(velocity));
	}
}
void PxPhysicsBody::setAngularVelocity(const glm::vec3 &velocity) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->setAngularVelocity(pxConvert(velocity));
	}
}
const F32 PxPhysicsBody::getMass() {
	return 1;
}
const glm::vec3 PxPhysicsBody::getScale() {
	//Not possible in PhysX
	return glm::vec3(1, 1, 1);
}
const glm::vec3 PxPhysicsBody::getLinearVelocity() {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		return pxConvert(dyn->getLinearVelocity());
	}
	return glm::vec3(0, 0, 0);
}
const glm::vec3 PxPhysicsBody::getAngularVelocity() {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		return pxConvert(dyn->getAngularVelocity());
	}
	return glm::vec3(0, 0, 0);
}