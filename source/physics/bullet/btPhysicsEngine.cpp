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

bool contactAdded(btManifoldPoint& cp,
				  const btCollisionObjectWrapper* colObj0Wrap,
				  int partId0,
				  int index0,
				  const btCollisionObjectWrapper* colObj1Wrap,
				  int partId1,
				  int index1) {

	//btAdjustInternalEdgeContacts(cp,colObj1Wrap,colObj0Wrap, partId1,index1);
	return true;
}

// This is where the FUN begins.
// dear god I hope I can multithread this shit.
void contactStarted(btPersistentManifold* const &manifold) {
    btBvhTriangleMeshShape *trimesh0 = dynamic_cast<btBvhTriangleMeshShape*>(const_cast<btCollisionShape*>(manifold->getBody0()->getCollisionShape()));
    btBvhTriangleMeshShape *trimesh1 = dynamic_cast<btBvhTriangleMeshShape*>(const_cast<btCollisionShape*>(manifold->getBody1()->getCollisionShape()));
    btBvhTriangleMeshShape *trimesh;
    const btCollisionObject *body, *otherBody;
    
    if (trimesh0 != NULL) {
        trimesh = trimesh0;
        body = manifold->getBody0();
        otherBody = manifold->getBody1();
    }
    else if (trimesh1 != NULL) {
        trimesh = trimesh1;
        body = manifold->getBody1();
        otherBody = manifold->getBody0();
    }
    else
        return;
    
    int ind = -1;
    for (int i = 0; i < bodies.size(); i++) {
        if (bodies[i].body == body) {
            ind = i;
            break;
        }
    }
    if (ind < 0)
        return;
    
    BodyInfo bodyInfo = bodies[ind];
    
    btTriangleMesh *mesh_int = (btTriangleMesh*)trimesh->getMeshInterface();
    
    std::vector<int> triangleIndices;
    bool removed = false;
    
    for (int i = 0; i < manifold->getNumContacts(); i++) {
        int index;
        if (trimesh0 != NULL)
            index = manifold->getContactPoint(i).m_index0;
        else
            index = manifold->getContactPoint(i).m_index1;
        
        for (int j = 0; j < triangleIndices.size(); j++) {
            if (std::find(bodyInfo.shape.adjacent.begin(), bodyInfo.shape.adjacent.end(), std::make_pair(triangleIndices[j], index)) != bodyInfo.shape.adjacent.end()) {
                if (manifold->getContactPoint(i).getDistance() < manifold->getContactPoint(j).getDistance())
                    manifold->removeContactPoint(j);
                else
                    manifold->removeContactPoint(i);
                printf("Point removed\n");
                removed = true;
                break;
            }
        }
        triangleIndices.push_back(index);
    }
    
    if (removed) {
        for (int i = 0; i < manifold->getNumContacts(); i++) {
            int index;
            if (trimesh0 != NULL)
                index = manifold->getContactPoint(i).m_index0;
            else
                index = manifold->getContactPoint(i).m_index1;
            const BulletTriangle &points = bodyInfo.shape.triangleData[index];
            btVector3 normal = (points.point1 - points.point0).cross(points.point2 - points.point0);
            normal.safeNormalize();
            
            btQuaternion rot = bodyInfo.body->getWorldTransform().getRotation();
            normal = normal.rotate(rot.getAxis(), rot.getAngle());
            
            if (trimesh0 != NULL)
                manifold->getContactPoint(i).m_normalWorldOnB = -normal;
            else
                manifold->getContactPoint(i).m_normalWorldOnB = normal;
        }
    }
}

void physicsWorldTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    btDispatcher *dispatcher = world->getDispatcher();
    
    S32 numManifolds = dispatcher->getNumManifolds();
    for (S32 i = 0; i < numManifolds; i++) {
        btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(i);
        // This is a callback ya know, we can do physics script callbacks in here.
        
        // clear manifold cache so it has to regenerate every tick. I know this doesn't
        // help performance but we need this for detecting real time collisions multiple times
        // so that we can ignore them.
        manifold->clearManifold();
    }
}

btPhysicsEngine::btPhysicsEngine() : PhysicsEngine() {
	init();
}

void btPhysicsEngine::init() {
	btDefaultCollisionConfiguration *configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);
	btBroadphaseInterface *interface = new btDbvtBroadphase();
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, interface, solver, configuration);
	world->setGravity(btVector3(0, 0, -20.0f));

	gContactAddedCallback = contactAdded;
    gContactStartedCallback = contactStarted;
    world->setInternalTickCallback(physicsWorldTickCallback);
	running = true;
}

void btPhysicsEngine::simulate(F32 delta) {
	if (running) {
		extraTime += delta;
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

	rigid->setCollisionFlags(rigid->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	world->addRigidBody(rigid);
}

PhysicsBody *btPhysicsEngine::createInterior(Interior *interior) {
	return new btPhysicsInterior(interior);
}

PhysicsBody *btPhysicsEngine::createSphere(F32 radius) {
	return new btPhysicsSphere(radius);
}

#endif
