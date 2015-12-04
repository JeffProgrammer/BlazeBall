//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/bullet/btPhysicsBody.h"
#include "physics/bullet/btPhysicsInterior.h"
#include "physics/bullet/btPhysicsSphere.h"
#include "physics/bullet/btDebugDrawer.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <algorithm>
#include <vector>

bool contactAddedCallback(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1) {
	btPhysicsBody *body0 = static_cast<btPhysicsBody *>(colObj0Wrap->getCollisionObject()->getUserPointer());
	btPhysicsBody *body1 = static_cast<btPhysicsBody *>(colObj1Wrap->getCollisionObject()->getUserPointer());

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
	btPhysicsBody *body0 = static_cast<btPhysicsBody *>(colObj0->getUserPointer());
	btPhysicsBody *body1 = static_cast<btPhysicsBody *>(colObj1->getUserPointer());

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

btPhysicsEngine::btPhysicsEngine() : PhysicsEngine() {
	init();

	extraTime = 0.0f;
}

void btPhysicsEngine::init() {
	btDefaultCollisionConfiguration *configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);
	btBroadphaseInterface *interface = new btDbvtBroadphase();
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	debugDrawer = new btDebugDrawer;

	world = new btDiscreteDynamicsWorld(dispatcher, interface, solver, configuration);
	world->setGravity(btVector3(0, 0, -20.0f));
	world->setDebugDrawer(debugDrawer);

	gContactAddedCallback = contactAddedCallback;
	gContactProcessedCallback = contactProcessedCallback;

	running = true;
}

void btPhysicsEngine::simulate(const F64 &delta) {
	if (running) {
		extraTime += delta;
		while (extraTime > PHYSICS_TICK) {
			step(PHYSICS_TICK);
			extraTime -= PHYSICS_TICK;
		}
		world->debugDrawWorld();
	}
}

void btPhysicsEngine::step(const F64 &delta) {
	PhysicsEngine::step(delta);
	world->stepSimulation(static_cast<btScalar>(delta), 10);
}

void btPhysicsEngine::addBody(PhysicsBody *body) {
	btPhysicsBody *physBody = static_cast<btPhysicsBody *>(body);

	btRigidBody *rigid = physBody->getActor();
	rigid->setUserPointer(body);

	rigid->setCollisionFlags(rigid->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	world->addRigidBody(rigid);
}

PhysicsBody *btPhysicsEngine::createInterior(GameInterior *interior) {
	return new btPhysicsInterior(interior);
}

PhysicsBody *btPhysicsEngine::createSphere(const F32 &radius) {
	return new btPhysicsSphere(radius);
}

void btPhysicsEngine::debugDraw(RenderInfo &info) {
	debugDrawer->draw(info);
}
