//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
	info.m_rollingFriction = 0.3f;

	//Create the actor and add it to the scene
	mActor = new btRigidBody(info);
	mActor->setActivationState(DISABLE_DEACTIVATION);
	mActor->setCcdMotionThreshold(static_cast<btScalar>(1e-3));
	mActor->setCcdSweptSphereRadius(radius / 10.0f);
	mActor->setRollingFriction(3.0f);
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

glm::vec3 btPhysicsSphere::getCollisionNormal(glm::vec3 &toiVelocity) {
	btDiscreteDynamicsWorld *world = static_cast<btPhysicsEngine *>(PhysicsEngine::getEngine())->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	glm::vec3 total = glm::vec3(0.0f, 0.0f, 0.0f);
	toiVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

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
				total += normal;
				toiVelocity += btConvert(manifold->getContactPoint(j).m_impactVelocity);
			}
		}
	}

	total = glm::normalize(total);
	toiVelocity = glm::normalize(toiVelocity);

	return total;
}

bool btPhysicsSphere::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	//The interior with which we collided
	btPhysicsInterior *inter = dynamic_cast<btPhysicsInterior *>(isBody0 ? info.body1 : info.body0);
	if (inter == nullptr)
		return true;

	//printf("vel: %f %f %f\n", sphere->getLinearVelocity().x, sphere->getLinearVelocity().y, sphere->getLinearVelocity().z);
	info.point.m_impactVelocity = btConvert(getLinearVelocity());

	/*
	 if ( collisionPoint.isOnTriSurface && !collisionPoint.isOnTriEdge )
		for each ( collisionPoint2 != collisionPoint in Manifold )
			if ( collisionPoint2.isOnTriEdge && collisionPoint2.triangle.isAdjacentTo( collisionPoint.triangle ) )
				collisionPoint2.removeFromManifold
	 */

	const btPhysicsInterior::TriangleInfo &triangleInfo = inter->getTriangleInfo(isBody0 ? info.index1 : info.index0);

	const DIF::Interior &dint = inter->getInterior()->getInterior(); //Encapsulation to the rescue
	const DIF::Interior::Surface &surf = dint.surface[triangleInfo.surfaceIndex];

	glm::vec3 normal = dint.normal[dint.plane[surf.planeIndex].normalIndex];
	if (surf.planeFlipped)
		normal *= -1.f;

	glm::vec3 btnormal = btConvert(info.point.m_normalWorldOnB);
	F32 distance = glm::distance(btnormal, normal);

//	printf("Actual normal %f %f %f\n", btnormal.x(), btnormal.y(), btnormal.z());
//	printf("Expected normal %f %f %f\n", normal.x, normal.y, normal.z);
//	printf("Distance: %f\n", btnormal.distance2(btConvert(normal)));

	if (distance < 0.001) {
		glm::vec3 collisionPoint = btConvert(info.point.m_positionWorldOnB);

		TriangleF triangle(dint.point[triangleInfo.vertex[0]], dint.point[triangleInfo.vertex[1]], dint.point[triangleInfo.vertex[2]]);
		if (triangle.isPointInside(collisionPoint) && !triangle.isPointOnEdge(collisionPoint)) {
			btDispatcher *dispatcher = static_cast<btPhysicsEngine *>(PhysicsEngine::getEngine())->getWorld()->getCollisionWorld()->getDispatcher();
			for (U32 i = 0; i < dispatcher->getNumManifolds(); i ++) {
				btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(i);

				std::vector<int> toRemove;

				if (manifold->getBody0() == mActor || manifold->getBody1() == mActor) {
					if (manifold->getNumContacts() > 1) {
						//Go through all the points for this manifold
						for (U32 j = 0; j < manifold->getNumContacts(); j ++) {
							btManifoldPoint &point = manifold->getContactPoint(j);
							//Don't check our point
							if (point.m_positionWorldOnB.distance2(info.point.m_positionWorldOnB) <= 0.0001f) {
								//Same point
								continue;
							}
							//Is the sphere 0 or 1?
							bool pointIsBody0 = (manifold->getBody0() == mActor);

							//See if their point qualifies
							const btPhysicsInterior::TriangleInfo &pointInfo = inter->getTriangleInfo(pointIsBody0 ? point.m_index1 : point.m_index0);

							//Is their point an edge?
							TriangleF pointTriangle(dint.point[pointInfo.vertex[0]], dint.point[pointInfo.vertex[1]], dint.point[pointInfo.vertex[2]]);
							if (pointTriangle.isPointOnEdge(btConvert(point.m_positionWorldOnB))) {
								//Todo: check adjacency
								if (pointTriangle.isTriangleAdjacent(triangle))
									toRemove.push_back(j);
							}
						}
						printf("Found one with %d contacts\n", manifold->getNumContacts());
					}
				}

				//Remove the points backwards
				for (auto it = toRemove.rbegin(); it != toRemove.rend(); it ++) {
					manifold->removeContactPoint(*it);
				}
			}
		}
	}

	//Texture names have properties
	const std::string &surfName = dint.materialName[surf.textureIndex];

	//Friction is relative to the slope of the incline
	F32 wallDot = info.point.m_normalWorldOnB.dot(btVector3(0, 0, 1));
	F32 friction = (1.0f + wallDot) / 2.0f;

	//Frictions
	if (stricmp(surfName.c_str(), "friction_low") == 0 ||
		stricmp(surfName.c_str(), "friction_low_shadow") == 0) {
		friction *= 0.2f;
	} else if (stricmp(surfName.c_str(), "friction_high") == 0 ||
			   stricmp(surfName.c_str(), "friction_high_shadow") == 0) {
		friction *= 2.5f;
	}

	info.point.m_combinedFriction *= friction;
	info.point.m_combinedRollingFriction *= friction;
	return true;
}

void btPhysicsSphere::notifyContact(ContactCallbackInfo &info, bool isBody0) {
	//How steep is the wall?
	F32 wallDot = info.point.m_normalWorldOnB.dot(btVector3(0, 0, 1));

	if ((wallDot * wallDot) < 0.0001f) {
		F32 appliedForce = sqrtf(glm::length(glm::proj(btConvert(info.point.m_impactVelocity), btConvert(info.point.m_normalWorldOnB))) * info.point.m_combinedFriction);
//		printf("Wall contact applied force %f\n", appliedForce);
		//Wall hit of some sort
//		applyImpulse(glm::vec3(0, 0, appliedForce), glm::vec3(0, 0, 0));
	}
}


F32 btPhysicsSphere::getRadius() {
	return static_cast<btSphereShape *>(mActor->getCollisionShape())->getRadius();
}

void btPhysicsSphere::setRadius(const F32 &radius) {
	static_cast<btSphereShape *>(mActor->getCollisionShape())->setUnscaledRadius(radius);
}
