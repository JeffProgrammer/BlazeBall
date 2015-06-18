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
#ifndef btPhysicsEngine_h
#define btPhysicsEngine_h

#define PHYSICS_TICK 0.016

#include <stdio.h>
#include "types.h"
#include "physicsEngine.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>

class btPhysicsEngine : public PhysicsEngine {
	btDiscreteDynamicsWorld *world;
	btCollisionDispatcher *dispatcher;
	bool running;
	F32 extraTime;
public:
	btPhysicsEngine();
	
	virtual void init();
	virtual void simulate(F32 delta);
	virtual void addBody(PhysicsBody *body);
	virtual PhysicsBody *createInterior(Interior *interior);
	virtual PhysicsBody *createSphere(F32 radius);

	btDiscreteDynamicsWorld *getWorld() { return world; };
	btCollisionDispatcher *getDispatcher() { return dispatcher; }
};

template <typename from, typename to>
static to btConvert(from point);

inline static const btVector3 btConvert(const Point3F &point) {
	return btVector3(point.x, point.y, point.z);
}

inline static const Point3F btConvert(const btVector3 &point) {
	return Point3F(point.x(), point.y(), point.z());
}

inline static const btQuaternion btConvert(const AngAxisF &point) {
	return btQuaternion(btConvert(point.axis), point.angle);
}

inline static const AngAxisF btConvert(const btQuaternion &point) {
	return AngAxisF(btConvert(point.getAxis()), point.getAngle());
}

#endif
#endif
