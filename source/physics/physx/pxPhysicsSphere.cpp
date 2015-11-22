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

#include "pxPhysicsSphere.h"

PxPhysicsSphere::PxPhysicsSphere(F32 radius) {
	physx::PxPhysics *physics = dynamic_cast<PxPhysicsEngine *>(PhysicsEngine::getEngine())->getPxPhysics();
	physx::PxMaterial *material = physics->createMaterial(1.1f, 0.9f, 0.8f);
	material->setFrictionCombineMode(physx::PxCombineMode::eMULTIPLY);
	material->setRestitutionCombineMode(physx::PxCombineMode::eMULTIPLY);

	mActor = physx::PxCreateDynamic(*physics, physx::PxTransform(0.0f, 0.0f, 0.0f), physx::PxSphereGeometry(radius), *material, 1.0f);
	physx::PxRigidBody *rigid = mActor->is<physx::PxRigidBody>();
	if (rigid) {
		physx::PxRigidBodyExt::setMassAndUpdateInertia(*rigid, 1.0f);
		rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
	}
	physx::PxRigidDynamic *dyn = mActor->is<physx::PxRigidDynamic>();
	if (dyn) {
		dyn->setMaxAngularVelocity(100.f);
	}
}

bool PxPhysicsSphere::getColliding() {
	//TODO collision detection
	return false;
}
glm::vec3 PxPhysicsSphere::getCollisionNormal(glm::vec3 &toiVelocity) {
	//TODO collision normals
	toiVelocity = glm::vec3(0, 0, 0);
	return glm::vec3(0, 0, 0);
}

F32 PxPhysicsSphere::getRadius() {
	return mRadius;
}

void PxPhysicsSphere::setRadius(const F32 &radius) {
	mRadius = radius;
	//TODO update
}
