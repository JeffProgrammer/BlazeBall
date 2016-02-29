//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <algorithm>
#include <vector>

#include "physics/physicsEngine.h"
#include "physics/physicsBody.h"
#include "physics/physicsSphere.h"

bool contactAddedCallback(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1) {
	PhysicsBody *body0 = static_cast<PhysicsBody *>(colObj0Wrap->getCollisionObject()->getUserPointer());
	PhysicsBody *body1 = static_cast<PhysicsBody *>(colObj1Wrap->getCollisionObject()->getUserPointer());

	ContactCallbackInfo info(cp);
	info.colObj0Wrap = colObj0Wrap;
	info.colObj1Wrap = colObj1Wrap;
	info.partId0 = partId0;
	info.partId1 = partId1;
	info.index0 = index0;
	info.index1 = index1;
	info.body0 = body0;
	info.body1 = body1;

	if (body0 && !body0->modifyContact(info, true))
		return false;
	if (body1 && !body1->modifyContact(info, false))
		return false;

	return true;
}

void contactProcessedCallback(btManifoldPoint &cp, const btCollisionObject *colObj0, const btCollisionObject *colObj1) {
	PhysicsBody *body0 = static_cast<PhysicsBody *>(colObj0->getUserPointer());
	PhysicsBody *body1 = static_cast<PhysicsBody *>(colObj1->getUserPointer());

	ContactCallbackInfo info(cp);
	info.colObj0 = colObj0;
	info.colObj1 = colObj1;
	info.partId0 = cp.m_partId0;
	info.partId1 = cp.m_partId1;
	info.index0 = cp.m_index0;
	info.index1 = cp.m_index1;
	info.body0 = body0;
	info.body1 = body1;

	if (body0)
		body0->notifyContact(info, true);
	if (body1)
		body1->notifyContact(info, false);
}

PhysicsEngine::PhysicsEngine() {
	mRunning = false;
	mExtraTime = 0.0;

	init();
}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::init() {
	btDefaultCollisionConfiguration *configuration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(configuration);
	btBroadphaseInterface *interface = new btDbvtBroadphase();
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	mWorld = new btDiscreteDynamicsWorld(mDispatcher, interface, solver, configuration);
	mWorld->setGravity(btVector3(0, 0, -20.0f));

#ifdef DEBUG_PHYSICS
	mDebugDrawer = new btDebugDrawer;
	mWorld->setDebugDrawer(mDebugDrawer);
#endif

	gContactAddedCallback = contactAddedCallback;
	gContactProcessedCallback = contactProcessedCallback;

	mRunning = true;
}

void PhysicsEngine::simulate(const F64 &delta) {
	if (mRunning) {
		mExtraTime += delta;
		while (mExtraTime > PHYSICS_TICK) {
			step(PHYSICS_TICK);
			mExtraTime -= PHYSICS_TICK;
		}

#ifdef DEBUG_PHYSICS
		mWorld->debugDrawWorld();
#endif
	}
}

void PhysicsEngine::step(const F64 &delta) {
	stepCallback(delta);
	mWorld->stepSimulation(static_cast<btScalar>(delta), 10);
}

void PhysicsEngine::addBody(PhysicsBody *physBody) {
	btRigidBody *rigid = physBody->getActor();
	rigid->setUserPointer(physBody);

	rigid->setCollisionFlags(rigid->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	mWorld->addRigidBody(rigid);
}

void PhysicsEngine::raycast(RaycastInfo &info) {

	btVector3 rayFrom = btConvert(info.from);
	btVector3 rayTo = btConvert(info.to);

	btCollisionWorld::ClosestRayResultCallback resultCallback(rayFrom, rayTo);
	mWorld->rayTest(rayFrom, rayTo, resultCallback);

	info.hit = resultCallback.hasHit();
	if (info.hit) {
		info.body = (void *)resultCallback.m_collisionObject;
		info.point = btConvert(resultCallback.m_hitPointWorld);
		info.normal = btConvert(resultCallback.m_hitNormalWorld);
	}
}