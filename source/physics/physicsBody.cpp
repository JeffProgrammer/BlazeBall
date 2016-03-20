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

BoxF PhysicsBody::getWorldBox() const {
	btVector3 min, max;
	mActor->getCollisionShape()->getAabb(mActor->getWorldTransform(), min, max);
	return BoxF(btConvert(min), btConvert(max));
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
