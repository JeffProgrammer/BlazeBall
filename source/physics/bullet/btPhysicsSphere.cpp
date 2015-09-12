//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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
#include "physics/bullet/btPhysicsSphere.h"
#include "physics/bullet/btPhysicsInterior.h"
#include "game/gameInterior.h"

#include <glm/ext.hpp>

#ifndef _WIN32
#define stricmp strcasecmp
#endif

/// BEHOLD, THE MAGIC NUMBER
#define ADJACENCY_NORMAL_THRESHOLD 0.01f

//Super crazy inheritence hacking to let us get at the private members of btTriangleMesh
class btAccessibleTriangleMesh : public btTriangleMesh {
public:
	static btVector3 getVector(btTriangleMesh *mesh, const U32 &index) {
		//Easy now
		return mesh->m_4componentVertices[mesh->m_32bitIndices[index]];
	}
	static BulletTriangle getTriangle(btTriangleMesh *mesh, const U32 &index) {
		BulletTriangle tri;
		tri.point0 = getVector(mesh, 0 + index * 3);
		tri.point1 = getVector(mesh, 1 + index * 3);
		tri.point2 = getVector(mesh, 2 + index * 3);
		return tri;
	}
	static bool areTrianglesAdjacent(const BulletTriangle &tri1, const BulletTriangle &tri2) {
		// Count number of matched vertices
		int matches = 0;
		if (tri1.point0 == tri2.point0 || tri1.point0 == tri2.point1 || tri1.point0 == tri2.point2)
			matches++;
		if (tri1.point1 == tri2.point0 || tri1.point1 == tri2.point1|| tri1.point1 == tri2.point2)
			matches++;
		if (tri1.point2 == tri2.point0 || tri1.point2 == tri2.point1 || tri1.point2 == tri2.point2)
			matches++;

		// Calculate normals
		btVector3 normal1 = (tri1.point1 - tri1.point0).cross(tri1.point2 - tri1.point0);
		btVector3 normal2 = (tri2.point1 - tri2.point0).cross(tri2.point2 - tri2.point0);
		normal1.safeNormalize();
		normal2.safeNormalize();
		btVector3 cross = normal1.cross(normal2);

		btScalar len = cross.length();
		if (len < ADJACENCY_NORMAL_THRESHOLD && matches >= 1) {
			return true;
		}
		return false;
	}
};

extern std::vector<ShapeInfo> shapes;
extern std::vector<BodyInfo> bodies;
extern std::vector<BodyMovement> moves;

btPhysicsSphere::btPhysicsSphere(const F32 &radius) : mRadius(radius) {
	//Motion state and shape
	btMotionState *state = new btDefaultMotionState();
	btCollisionShape *shape = new btSphereShape(radius);

	//Need this otherwise forces won't work!
	btVector3 fallInertia = btVector3(0.f, 0.f, 0.f);
	shape->calculateLocalInertia(1.0f, fallInertia);
	shape->setMargin(0.01f);

	//Update position
	btTransform transform;
	transform.setIdentity();

	state->setWorldTransform(transform);

	//Construction info
	btRigidBody::btRigidBodyConstructionInfo info(1, state, shape, fallInertia);
	info.m_restitution = 0.5f; // 0.5 * 0.7
	info.m_friction = 1.0f;
	info.m_rollingFriction = 1.1f;

	//Create the actor and add it to the scene
	mActor = new btRigidBody(info);
	mActor->setActivationState(DISABLE_DEACTIVATION);
	mActor->setCcdMotionThreshold(static_cast<btScalar>(1e-3));
	mActor->setCcdSweptSphereRadius(radius / 10.0f);
	mActor->setAnisotropicFriction(shape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	mActor->setContactProcessingThreshold(0.0f);

	ShapeInfo infooo;
	infooo.shape = shape;
	shapes.push_back(infooo);
    
	BodyInfo infoo;
	infoo.body = mActor;
	infoo.collisionNormal = btVector3(0.0f, 0.0f, 0.0f);
	infoo.isDynamic = true;
	infoo.shape = infooo;
	bodies.push_back(infoo);
}

bool btPhysicsSphere::getColliding() {
	btDiscreteDynamicsWorld *world = static_cast<btPhysicsEngine *>(PhysicsEngine::getEngine())->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	for (U32 i = 0; i < manifolds; i ++) {
		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();

		if (obj1 == mActor || obj2 == mActor) {
			if (manifold->getNumContacts() > 0)
				return true;
		}
	}

	return false;
}

glm::vec3 btPhysicsSphere::getCollisionNormal() {
	btDiscreteDynamicsWorld *world = static_cast<btPhysicsEngine *>(PhysicsEngine::getEngine())->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	glm::vec3 best = glm::vec3(0.0f, 0.0f, 0.0f);
	F32 dot = 0;

	for (U32 i = 0; i < manifolds; i ++) {
		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();

		if (obj1 == mActor || obj2 == mActor) {
			U32 contacts = manifold->getNumContacts();
			for (U32 j = 0; j < contacts; j ++) {
				glm::vec3 normal = btConvert(manifold->getContactPoint(j).m_normalWorldOnB);
				if (obj2 == mActor)
					normal *= -1;
				if (glm::dot(normal, glm::vec3(0, 0, 1)) > dot) {
					best = normal;
					dot = glm::dot(normal, glm::vec3(0, 0, 1));
				}
			}
		}
	}

	return best;
}

bool btPhysicsSphere::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	//The interior with which we collided
	btPhysicsInterior *inter = dynamic_cast<btPhysicsInterior *>(isBody0 ? info.body1 : info.body0);
	if (inter == nullptr)
		return false;

	const DIF::Interior &dint = inter->getInterior()->getInterior(); //Encapsulation to the rescue

	//Which surface was it?
	U32 surfaceNum = inter->getSurfaceIndexFromTriangleIndex(isBody0 ? info.index1 : info.index0);
	const DIF::Interior::Surface &surface = dint.surface[surfaceNum];

	const DIF::Point3F &normal = dint.normal[dint.plane[surface.planeIndex].normalIndex];

	glm::vec3 in(normal.x, normal.y, normal.z);
	glm::vec3 bn = btConvert(info.point.m_normalWorldOnB);

	//Doesn't really work
//	if (glm::dot(in, bn) < 0.95) {
//		//Remove it
//		return false;
//	}


	//Texture names have properties
	const std::string &surfName = dint.materialName[surface.textureIndex];

	//Friction is relative to the slope of the incline
	F32 friction = (1.0f + info.point.m_normalWorldOnB.dot(btVector3(0, 0, 1))) / 2.0f;

	//Frictions
	if (stricmp(surfName.c_str(), "friction_low") == 0 ||
		stricmp(surfName.c_str(), "friction_low_shadow") == 0) {
		friction *= 0.01f;
	} else if (stricmp(surfName.c_str(), "friction_high") == 0 ||
			   stricmp(surfName.c_str(), "friction_high_shadow") == 0) {
		friction *= 2.5f;
	}

	info.point.m_combinedFriction *= friction;
	info.point.m_combinedRollingFriction *= friction;

	return true;
}

F32 btPhysicsSphere::getRadius() {
	return static_cast<btSphereShape *>(mActor->getCollisionShape())->getRadius();
}

void btPhysicsSphere::setRadius(const F32 &radius) {
	static_cast<btSphereShape *>(mActor->getCollisionShape())->setUnscaledRadius(radius);
}
