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

void btPhysicsSphere::modifyContact(btPersistentManifold *const &manifold, const btCollisionObject *other, U32 otherIndex) {
	const btBvhTriangleMeshShape *trimesh = dynamic_cast<const btBvhTriangleMeshShape *>(other->getCollisionShape());

	if (trimesh == NULL)
		return;

	int ind = -1;
	for (size_t i = 0; i < bodies.size(); i++) {
		if (bodies[i].body == other) {
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
	int count = manifold->getNumContacts();

	bool *removedPoints = new bool[count];
	memset(removedPoints, 0, count * sizeof(bool));

	int upCount = 0;

	for (int i = 0; i < count; i++) {
		int index;
		if (otherIndex == 0)
			index = manifold->getContactPoint(i).m_index0;
		else
			index = manifold->getContactPoint(i).m_index1;

		for (size_t j = 0; j < triangleIndices.size(); j++) {
			assert(j < count);
			if (removedPoints[j])
				continue;

			BulletTriangle tri1 = btAccessibleTriangleMesh::getTriangle(mesh_int, triangleIndices[j]);
			BulletTriangle tri2 = btAccessibleTriangleMesh::getTriangle(mesh_int, index);

			if (index == triangleIndices[j] || btAccessibleTriangleMesh::areTrianglesAdjacent(tri1, tri2)) {
				if (manifold->getContactPoint(i).getDistance() < manifold->getContactPoint(j).getDistance()) {
					//manifold->removeContactPoint(j);
					removedPoints[j + upCount] = true;
					assert(j + upCount < count);
				}
				else {
					//manifold->removeContactPoint(i);
					removedPoints[i + upCount] = true;
					assert(i + upCount < count);
				}
				removed = true;
				upCount++;
				break;
			}
		}
		triangleIndices.push_back(index);
	}

	for (int i = count - 1; i >= 0; i--) {
		if (removedPoints[i])
			manifold->removeContactPoint(i);
	}

	delete[] removedPoints;

	if (removed) {
		for (int i = 0; i < manifold->getNumContacts(); i++) {
			int index;
			if (otherIndex == 0)
				index = manifold->getContactPoint(i).m_index0;
			else
				index = manifold->getContactPoint(i).m_index1;
			const BulletTriangle &points = btAccessibleTriangleMesh::getTriangle(mesh_int, index);

			btVector3 normal = (points.point1 - points.point0).cross(points.point2 - points.point0);
			normal.safeNormalize();

			btQuaternion rot = bodyInfo.body->getWorldTransform().getRotation();
			normal = normal.rotate(rot.getAxis(), rot.getAngle());

			if (otherIndex == 0)
				manifold->getContactPoint(i).m_normalWorldOnB = -normal;
			else
				manifold->getContactPoint(i).m_normalWorldOnB = normal;
		}
	}

	//The interior with which we collided
	btPhysicsInterior *inter = static_cast<btPhysicsInterior *>(other->getUserPointer());
	const DIF::Interior &dint = inter->getInterior()->getInterior(); //Encapsulation to the rescue

	for (int i = 0; i < manifold->getNumContacts(); i ++) {
		//Get the collision index
		U32 index;
		if (otherIndex == 0)
			index = manifold->getContactPoint(i).m_index0;
		else
			index = manifold->getContactPoint(i).m_index1;

		//Which surface was it?
		U32 surfaceNum = inter->getSurfaceIndexFromTriangleIndex(index);
		DIF::Interior::Surface surface = dint.surface[surfaceNum];

		//Texture names have properties
		std::string surfName = dint.materialName[surface.textureIndex];

		//Friction is relative to the slope of the incline
		F32 friction = (1.0f + manifold->getContactPoint(i).m_normalWorldOnB.dot(btVector3(0, 0, 1))) / 2.0f;

		//Frictions
		if (surfName == "friction_low" || surfName == "friction_low_shadow") {
			friction *= 0.01f;
		} else if (surfName == "friction_high" || surfName == "friction_high_shadow") {
			friction *= 2.5f;
		}

		manifold->getContactPoint(i).m_combinedFriction *= friction;
		manifold->getContactPoint(i).m_combinedRollingFriction *= friction;
	}
}

F32 btPhysicsSphere::getRadius() {
	return dynamic_cast<btSphereShape *>(mActor->getCollisionShape())->getRadius();
}

void btPhysicsSphere::setRadius(const F32 &radius) {
	dynamic_cast<btSphereShape *>(mActor->getCollisionShape())->setUnscaledRadius(radius);
}
