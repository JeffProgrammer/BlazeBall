//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_BULLET_BTPHYSICSENGINE_H_
#define _GAME_PHYSICS_BULLET_BTPHYSICSENGINE_H_

#include <stdio.h>
#include "base/types.h"
#include "physics/physicsEngine.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <vector>

struct BulletTriangle {
    btVector3 point0;
    btVector3 point1;
    btVector3 point2;
};

class btPhysicsBody;

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

	btPhysicsBody *body0;
	btPhysicsBody *body1;

	ContactCallbackInfo(btManifoldPoint &pt) : point(pt) {};
};

class btDebugDrawer;

class btPhysicsEngine : public PhysicsEngine {
	btDiscreteDynamicsWorld *mWorld;
	btCollisionDispatcher *mDispatcher;
#ifdef DEBUG_PHYSICS
	btDebugDrawer *mDebugDrawer;
#endif

	F64 mExtraTime;

	virtual void step(const F64 &delta);
public:
	btPhysicsEngine();
	
	virtual void init();
	virtual void simulate(const F64 &delta);
	virtual void addBody(PhysicsBody *body);
	virtual void raycast(RaycastInfo &info);
	virtual void debugDraw(RenderInfo &info, const DebugDrawType &drawType);
	virtual PhysicsBody *createInterior(GameInterior *interior);
	virtual PhysicsBody *createSphere(const F32 &radius);

	btDiscreteDynamicsWorld *getWorld() { return mWorld; };
	btCollisionDispatcher *getDispatcher() { return mDispatcher; }
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

#endif // _GAME_PHYSICS_BULLET_BTPHYSICSENGINE_H_