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

#include "physics/bullet/btPhysicsBody.h"

const glm::vec3 btPhysicsBody::getPosition() {
	return btConvert(mActor->getWorldTransform().getOrigin());
}
const AngAxisF btPhysicsBody::getRotation() {
	return btConvert(mActor->getWorldTransform().getRotation());
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
void btPhysicsBody::setRotation(const AngAxisF &rotation) {
	btTransform worldTrans = mActor->getWorldTransform();
	worldTrans.setRotation(btConvert(rotation));
	mActor->setWorldTransform(worldTrans);
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