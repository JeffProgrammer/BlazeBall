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

#ifndef pxPhysicsEngine_h
#define pxPhysicsEngine_h

#if defined(DEBUG)
#define _DEBUG
#elif !defined(_DEBUG)
#define NDEBUG
#endif

#include "physics/physicsEngine.h"
#include <glm/glm.hpp>

#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"

class PxPhysicsEngine : public PhysicsEngine {
protected:
	physx::PxFoundation *foundation;
	physx::PxProfileZoneManager *profileZoneManager;
	physx::PxPhysics *physics;
	physx::PxCooking *cooking;
	physx::PxScene *scene;
	physx::PxCpuDispatcher *dispatcher;

	F64 mExtraTime;

public:
	PxPhysicsEngine() : mExtraTime(0.0) {}

	virtual void init();
	virtual void destroy();
	virtual void simulate(const F64 &delta);
	virtual void addBody(PhysicsBody *body);

	virtual PhysicsBody *createInterior(GameInterior *interior);
	virtual PhysicsBody *createSphere(const F32 &radius);

	physx::PxPhysics *getPxPhysics() { return physics; }
	physx::PxCooking *getPxCooking() { return cooking; }

	//	btDiscreteDynamicsWorld *getWorld() { return world; };
	//	btCollisionDispatcher *getDispatcher() { return dispatcher; }
	
};

class AllocatorCallback : public physx::PxAllocatorCallback {
public:
	virtual void *allocate(size_t size, const char *typeName, const char *fileName, int line) {
#ifdef _WIN32
		return _aligned_malloc(size, 16);
#else
		return malloc(size);
#endif
	}
	virtual void deallocate(void *ptr) {
#ifdef _WIN32
		_aligned_free(ptr);
#else
		free(ptr);
#endif
	}
};

class ErrorCallback : public physx::PxErrorCallback {
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {
		printf("Physx Error: %d %s [File: %s (%d)]\n", code, message, file, line);
	}
};

template <typename from, typename to>
to pxConvert(from val);

inline glm::vec3 pxConvert(const physx::PxVec3 &val) {
	return glm::vec3(-val.x, -val.y, val.z);
}

inline physx::PxVec3 pxConvert(const glm::vec3 &val) {
	return physx::PxVec3(-val.x, -val.y, val.z);
}

inline glm::quat pxConvert(const physx::PxTransform &val) {
	return glm::quat(val.q.x, val.q.y, val.q.z, val.q.w);
}

inline physx::PxQuat pxConvert(const glm::quat &quat) {
	return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
}

#endif
