//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/bullet/btPhysicsSphere.h"
#include "physics/bullet/btPhysicsInterior.h"
#include "game/gameInterior.h"
#include "base/math/triangle.h"
#include <glm/ext.hpp>

#ifndef _WIN32
#define stricmp strcasecmp
#endif

btPhysicsSphere::btPhysicsSphere(const F32 &radius) {
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

	U32 index = isBody0 ? info.index1 : info.index0;

	//printf("vel: %f %f %f\n", sphere->getLinearVelocity().x, sphere->getLinearVelocity().y, sphere->getLinearVelocity().z);
	info.point.m_impactVelocity = btConvert(getLinearVelocity());

	const btPhysicsInterior::TriangleInfo &triangleInfo = inter->getTriangleInfo(index);

	const DIF::Interior &dint = inter->getInterior()->getInterior(); //Encapsulation to the rescue
	const DIF::Interior::Surface &surf = dint.surface[triangleInfo.surfaceIndex];

	glm::vec3 normal = dint.normal[dint.plane[surf.planeIndex].normalIndex];
	if (surf.planeFlipped)
		normal *= -1.f;

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
	//The interior with which we collided
	btPhysicsInterior *inter = dynamic_cast<btPhysicsInterior *>(isBody0 ? info.body1 : info.body0);
	if (inter == nullptr)
		return;

	/*
	 via https://github.com/bulletphysics/bullet3/issues/288

	 if ( collisionPoint.isOnTriSurface && !collisionPoint.isOnTriEdge )
		for each ( collisionPoint2 != collisionPoint in Manifold )
			 if ( collisionPoint2.isOnTriEdge && collisionPoint2.triangle.isAdjacentTo( collisionPoint.triangle ) )
				 collisionPoint2.removeFromManifold
	 */

	//Easier access
	U32 index = isBody0 ? info.index1 : info.index0;

	//Get some information about the interior
	const btPhysicsInterior::TriangleInfo &triangleInfo = inter->getTriangleInfo(index);
	const DIF::Interior &dint = inter->getInterior()->getInterior(); //Encapsulation to the rescue

	glm::vec3 collisionPoint = btConvert(info.point.m_positionWorldOnB);

	//Triangle info from the interior
	TriangleF triangle(dint.point[triangleInfo.vertex[0]], dint.point[triangleInfo.vertex[1]], dint.point[triangleInfo.vertex[2]]);

	//We need to remove points if this isn't an edge collision
	if (triangle.isPointInside(collisionPoint) && !triangle.isPointOnEdge(collisionPoint)) {

		//Test all other manifold points
		btDispatcher *dispatcher = static_cast<btPhysicsEngine *>(PhysicsEngine::getEngine())->getWorld()->getCollisionWorld()->getDispatcher();
		for (U32 i = 0; i < dispatcher->getNumManifolds(); i ++) {
			btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(i);

			//Store a list of points to remove so we don't get weird memory errors
			std::vector<int> toRemove;

			//Is the sphere 0 or 1?
			bool pointIsBody0 = (manifold->getBody0() == mActor);

			//Don't care about manifolds that we aren't part of
			if (manifold->getBody0() == mActor || manifold->getBody1() == mActor) {
				//Don't bother checking if we're the only point in that manifold
				if (manifold->getNumContacts() > 1) {
					//Go through all the points for this manifold
					for (U32 j = 0; j < manifold->getNumContacts(); j ++) {
						btManifoldPoint &point = manifold->getContactPoint(j);

						//Access to index
						U32 pointIndex = (pointIsBody0 ? point.m_index1 : point.m_index0);

						//See if their point qualifies
						const btPhysicsInterior::TriangleInfo &pointInfo = inter->getTriangleInfo(pointIndex);

						//Don't check our point
						if (point.m_positionWorldOnB.distance2(info.point.m_positionWorldOnB) <= 0.0001f &&
							point.m_normalWorldOnB.distance2(info.point.m_normalWorldOnB) <= 0.0001f) {
							//Same point; ignore
							continue;
						}

						//Is their point an edge?
						TriangleF pointTriangle(dint.point[pointInfo.vertex[0]], dint.point[pointInfo.vertex[1]], dint.point[pointInfo.vertex[2]]);
						if (pointTriangle.isPointOnEdge(btConvert(point.m_positionWorldOnB))) {
							//Only remove their point if it's adjacent to ours
							if (pointTriangle.isTriangleAdjacent(triangle)) {
								//Yep, remove it
								toRemove.push_back(j);
								continue;
							}
						}
					}
				}
			}

			//Remove the points backwards so we don't get weird memory errors
			for (auto it = toRemove.rbegin(); it != toRemove.rend(); it ++) {
				manifold->removeContactPoint(*it);
			}
		}
	}

	//How steep is the wall?
	F32 wallDot = info.point.m_normalWorldOnB.dot(btVector3(0, 0, 1));

	if ((wallDot * wallDot) < 0.0001f) {
//		F32 appliedForce = sqrtf(glm::length(glm::proj(btConvert(info.point.m_impactVelocity), btConvert(info.point.m_normalWorldOnB))) * info.point.m_combinedFriction);
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
