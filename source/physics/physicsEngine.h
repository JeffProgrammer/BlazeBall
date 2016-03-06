//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
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