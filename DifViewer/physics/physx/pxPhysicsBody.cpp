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

const Point3F PxPhysicsBody::getPosition() {
	return pxConvert(mActor->getGlobalPose().p);
}
const AngAxisF PxPhysicsBody::getRotation() {
	return pxConvert(mActor->getGlobalPose());
}

void PxPhysicsBody::setMass(const F32 &mass) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->setMass(mass);
	}
}
void PxPhysicsBody::setPosition(const Point3F &position) {
	physx::PxTransform trans;
	trans.p = pxConvert(position);
	trans.q = mActor->getGlobalPose().q;
	mActor->setGlobalPose(trans);
}
void PxPhysicsBody::setRotation(const AngAxisF &rotation) {
	physx::PxTransform trans;
	trans.p = mActor->getGlobalPose().p;
	trans.q = pxConvert(rotation);
	mActor->setGlobalPose(trans);
}
void PxPhysicsBody::applyTorque(const Point3F &torque) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		dyn->addTorque(pxConvert(torque));
	}
}
void PxPhysicsBody::applyImpulse(const Point3F &impulse, const Point3F &origin) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		physx::PxRigidBodyExt::addForceAtPos(*dyn, pxConvert(impulse), pxConvert(origin));
	}
}
void PxPhysicsBody::applyForce(const Point3F &force, const Point3F &origin) {
	if (getDynamic()) {
		physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
		physx::PxRigidBodyExt::addForceAtPos(*dyn, pxConvert(force), pxConvert(origin));
	}
}

bool PxPhysicsBody::getDynamic() {
	return mActor->is<physx::PxRigidDynamic>();
}
