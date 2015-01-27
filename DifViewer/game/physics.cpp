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

#ifdef BUILD_PHYSICS
#include "physics.h"

void Physics::init() {
	static AllocatorCallback *allocator = nullptr;
	if (allocator == nullptr) {
		allocator = new AllocatorCallback();
	}
	static ErrorCallback *errorer = nullptr;
	if (errorer == nullptr) {
		errorer = new ErrorCallback();
	}
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, *errorer);
	profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(foundation);

	bool recordMemoryAllocations = true;
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), recordMemoryAllocations, profileZoneManager);
	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale()));

	PxInitExtensions(*physics);

	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity =	physx::PxVec3(0, 0, -20.f);

	if (!sceneDesc.cpuDispatcher) {
		dispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = dispatcher;
	}
	scene = physics->createScene(sceneDesc);

	running = true;
}

void Physics::destroy() {
	physics->release();
	foundation->release();
}

void Physics::simulate(F32 delta) {
	if (running) {
		scene->fetchResults();
		extraTime += delta;
		for ( ; extraTime > PHYSICS_TICK; extraTime -= PHYSICS_TICK) {
			scene->simulate(extraTime);
		}
		scene->simulate(extraTime);
		extraTime = 0;
	}
}

Physics *Physics::getPhysics() {
	static Physics *physics = nullptr;
	if (physics == nullptr) {
		physics = new Physics;
	}
	return physics;
}

void Physics::addActor(physx::PxActor *actor) {
	scene->addActor(*actor);
}

#endif
