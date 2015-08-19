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

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/bullet/btPhysicsBody.h"
#include "physics/bullet/btPhysicsInterior.h"
#include "physics/bullet/btPhysicsSphere.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>
#include <algorithm>
#include <vector>

std::vector<ShapeInfo> shapes;
std::vector<BodyInfo> bodies;
std::vector<BodyMovement> moves;

// This is where the FUN begins.
// dear god I hope I can multithread this shit.
void contactStarted(btPersistentManifold* const &manifold) {
    btPhysicsBody *body0 = static_cast<btPhysicsBody *>(manifold->getBody0()->getUserPointer());
    btPhysicsBody *body1 = static_cast<btPhysicsBody *>(manifold->getBody1()->getUserPointer());

	if (body0)
		body0->modifyContact(manifold, manifold->getBody1(), 1);
	if (body1)
		body1->modifyContact(manifold, manifold->getBody0(), 0);
}

void physicsWorldTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    //btDispatcher *dispatcher = world->getDispatcher();
    
    //S32 numManifolds = dispatcher->getNumManifolds();
    //for (S32 i = 0; i < numManifolds; i++) {
        //btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(i);
        // This is a callback ya know, we can do physics script callbacks in here.
        
        // clear manifold cache so it has to regenerate every tick. I know this doesn't
        // help performance but we need this for detecting real time collisions multiple times
        // so that we can ignore them.
//        manifold->clearManifold();
    //}
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

    gContactStartedCallback = contactStarted;
    //world->setInternalTickCallback(physicsWorldTickCallback);
	running = true;
}

void btPhysicsEngine::simulate(const F64 &delta) {
	if (running) {
		extraTime += delta / 1000.0;
		for ( ; extraTime > PHYSICS_TICK; extraTime -= PHYSICS_TICK) {
			world->stepSimulation(PHYSICS_TICK, 10);
		}
		world->stepSimulation(extraTime);
		extraTime = 0;
	}
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
