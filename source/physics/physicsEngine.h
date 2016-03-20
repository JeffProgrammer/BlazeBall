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

#ifndef _GAME_PHYSICS_PHYSICSENGINE_H_
#define _GAME_PHYSICS_PHYSICSENGINE_H_

#include <stdio.h>
#include <functional>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include "base/types.h"
#include "render/renderInfo.h"

#define PHYSICS_TICK 0.01666

class PhysicsBody;
class GameInterior;

struct BulletTriangle {
	btVector3 point0;
	btVector3 point1;
	btVector3 point2;
};

struct ContactCallbackInfo {
	btManifoldPoint &point;

	union {
		const btCollisionObjectWrapper *colObj0Wrap;
		const btCollisionObject *colObj0;
	};
	union {
		const btCollisionObjectWrapper *colObj1Wrap;
		const btCollisionObject *colObj1;
	};
	int partId0;
	int partId1;
	int index0;
	int index1;

	PhysicsBody *body0;
	PhysicsBody *body1;

	ContactCallbackInfo(btManifoldPoint &pt) : point(pt) {};
};

class PhysicsEngine {
public:
	typedef std::function<void(const F64 &)> StepCallback;
protected:
	bool mRunning;
	F64 mExtraTime;

	btDiscreteDynamicsWorld *mWorld;
	btCollisionDispatcher *mDispatcher;
#ifdef DEBUG_PHYSICS
	btDebugDrawer *mDebugDrawer;
#endif

	StepCallback stepCallback;

	void step(const F64 &delta);
public:
	PhysicsEngine();
	~PhysicsEngine();

	void init();
	void simulate(const F64 &delta);
	void addBody(PhysicsBody *body);

	void setRunning(bool running) { this->mRunning = running; }
	bool getRunning() { return mRunning; }

	btDiscreteDynamicsWorld *getWorld() { return mWorld; };
	btCollisionDispatcher *getDispatcher() { return mDispatcher; }

	void setStepCallback(StepCallback callback) { this->stepCallback = callback; }

	struct RaycastInfo {
		glm::vec3 from;
		glm::vec3 to;
		bool hit;

		glm::vec3 point;
		glm::vec3 normal;		
		void *body;
	};

	void raycast(RaycastInfo &info);

	enum DebugDrawType {
		Everything,
		Nothing
	};

	void debugDraw(RenderInfo &info, const DebugDrawType &drawType) {
		// TODO
	}
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

#endif // _GAME_PHYSICS_PHYSICSENGINE_H_