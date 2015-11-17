//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// Copyright (c) 2015 Whirligig231
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/bullet/btPhysicsBody.h"
#include "physics/bullet/btPhysicsInterior.h"
#include "physics/bullet/btPhysicsSphere.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <algorithm>
#include <vector>

std::vector<ShapeInfo> shapes;
std::vector<BodyInfo> bodies;
std::vector<BodyMovement> moves;

bool contact_added_callback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
	btAdjustInternalEdgeContacts(cp, colObj0Wrap, colObj1Wrap, partId1, index1, BT_TRIANGLE_CONVEX_DOUBLE_SIDED);
	btAdjustInternalEdgeContacts(cp, colObj1Wrap, colObj0Wrap, partId0, index0, BT_TRIANGLE_CONVEX_DOUBLE_SIDED);

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

btPhysicsEngine::btPhysicsEngine() : PhysicsEngine() {
	init();

	extraTime = 0.0f;
}

void btPhysicsEngine::init() {
	btDefaultCollisionConfiguration *configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);
	btBroadphaseInterface *interface = new btDbvtBroadphase();
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, interface, solver, configuration);
	world->setGravity(btVector3(0, 0, -20.0f));

	gContactAddedCallback = contact_added_callback;
	running = true;
}

void btPhysicsEngine::simulate(const F64 &delta) {
	if (running) {
		extraTime += delta;
		while (extraTime > PHYSICS_TICK) {
			step(PHYSICS_TICK);
			extraTime -= PHYSICS_TICK;
		}
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
