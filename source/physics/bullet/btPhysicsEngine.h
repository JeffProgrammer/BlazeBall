//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// Copyright (c) 2015 Whirligig231
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

#ifndef btPhysicsEngine_h
#define btPhysicsEngine_h

#define PHYSICS_TICK 0.01666

#include <stdio.h>
#include "base/types.h"
#include "physics/physicsEngine.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <vector>

struct BulletTriangle {
    btVector3 point0;
    btVector3 point1;
    btVector3 point2;
};

struct ShapeInfo {
    btCollisionShape *shape;
};

struct BodyInfo {
    btRigidBody* body;
    ShapeInfo shape;
    btVector3 collisionNormal;
    btVector3 collisionNormalTemp;
    bool isDynamic;
    float timeout;

	 BodyInfo() : isDynamic(false), timeout(0.0f) {}
};

struct BodyMovement {
    btRigidBody *body;
    btVector3 velocity;
    btVector3 angularVelocity;
};

class btPhysicsEngine : public PhysicsEngine {
	btDiscreteDynamicsWorld *world;
	btCollisionDispatcher *dispatcher;
	bool running;
	F64 extraTime;

	virtual void step(F64 delta);
public:
	btPhysicsEngine();
	
	virtual void init();
	virtual void simulate(const F64 &delta);
	virtual void addBody(PhysicsBody *body);
	virtual PhysicsBody *createInterior(GameInterior *interior);
	virtual PhysicsBody *createSphere(const F32 &radius);

	btDiscreteDynamicsWorld *getWorld() { return world; };
	btCollisionDispatcher *getDispatcher() { return dispatcher; }
};

template <typename from, typename to>
static to btConvert(from point);

inline static const btVector3 btConvert(const glm::vec3 &point) {
	return btVector3(point.x, point.y, point.z);
}

inline static const glm::vec3 btConvert(const btVector3 &point) {
	return glm::vec3(point.x(), point.y(), point.z());
}

inline static const btQuaternion btConvert(const glm::quat &point) {
	return btQuaternion(point.w, point.x, point.y, point.z);
}

inline static const glm::quat btConvert(const btQuaternion &point) {
	return glm::quat(point.w(), point.x(), point.y(), point.z());
}

#endif