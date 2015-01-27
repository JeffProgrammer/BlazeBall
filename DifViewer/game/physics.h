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
#ifndef physics_h
#define physics_h

#define PHYSICS_TICK 0.016

#include <stdio.h>
#include "types.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"

class Physics {
	physx::PxFoundation *foundation;
	physx::PxProfileZoneManager *profileZoneManager;
	physx::PxPhysics *physics;
	physx::PxCooking *cooking;
	physx::PxScene *scene;
	physx::PxCpuDispatcher *dispatcher;

	bool running;
	F32 extraTime;
public:
	void init();
	void destroy();
	void simulate(F32 delta);
	void addActor(physx::PxActor *actor);
	void setRunning(bool running) { this->running = running; }
	bool getRunning() { return running; }

	physx::PxPhysics *getPxPhysics() { return physics; }
	physx::PxCooking *getPxCooking() { return cooking; }

//	btDiscreteDynamicsWorld *getWorld() { return world; };
//	btCollisionDispatcher *getDispatcher() { return dispatcher; }

	static Physics *getPhysics();
};

class AllocatorCallback : public physx::PxAllocatorCallback {
	virtual void *allocate(size_t size, const char *typeName, const char *fileName, int line) {
		return malloc(size);
	}
	virtual void deallocate(void *ptr) {
		free(ptr);
	}
};

class ErrorCallback : public physx::PxErrorCallback {
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {
		printf("Physx Error: %d %s [File: %s (%d)]\n", code, message, file, line);
	}
};

template <typename from, typename to>
to pxConvert(from val);

template<> inline Point3F pxConvert(physx::PxVec3 val) {
	return Point3F(val.x, val.y, val.z);
}

template<> inline physx::PxVec3 pxConvert(Point3F val) {
	return physx::PxVec3(val.x, val.y, val.z);
}

template<> inline QuatF pxConvert(physx::PxTransform val) {
	return QuatF(val.q.x, val.q.y, val.q.z, val.q.w);
}

template<> inline QuatF pxConvert(physx::PxQuat val) {
	return QuatF(val.x, val.y, val.z, val.w);
}

template<> inline Point3F pxConvert(physx::PxTransform val) {
	return Point3F(val.p.x, val.p.y, val.p.z);
}

template<> inline physx::PxTransform pxConvert(Point3F val) {
	return physx::PxTransform(val.x, val.y, val.z);
}

template<> inline physx::PxQuat pxConvert(QuatF val) {
	return physx::PxQuat(val.x, val.y, val.z, val.w);
}

#endif
#endif
