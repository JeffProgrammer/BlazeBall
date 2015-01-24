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

#include "physics.h"	
#include <BulletCollision/CollisionShapes/btTriangleShape.h>

bool contactAdded(btManifoldPoint& cp,
				  const btCollisionObjectWrapper* colObj0Wrap,
				  int partId0,
				  int index0,
				  const btCollisionObjectWrapper* colObj1Wrap,
				  int partId1,
				  int index1) {



	return true;
}

void Physics::init() {
	btDefaultCollisionConfiguration *configuration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(configuration);
	btBroadphaseInterface *interface = new btDbvtBroadphase();
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, interface, solver, configuration);
	world->setGravity(btVector3(0, 0, -30.0f));

	gContactAddedCallback = contactAdded;
	
}

void Physics::simulate(F32 delta) {
	world->stepSimulation(delta);
}

Physics *Physics::getPhysics() {
	static Physics *physics = nullptr;
	if (physics == nullptr) {
		physics = new Physics;
	}
	return physics;
}

void Physics::addRigidBody(btRigidBody *body) {
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	world->addRigidBody(body);
}
