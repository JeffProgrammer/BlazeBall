//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#include "physics/physicsSphere.h"
#include "physics/physicsEngine.h"
#include "physics/physicsInterior.h"

#include "game/gameInterior.h"
#include "game/sphere.h"
#include "base/math/triangle.h"

#ifndef _WIN32
#define stricmp strcasecmp
#endif

PhysicsSphere::PhysicsSphere(Sphere *sphere, const F32 &radius) {
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
	mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	mActor->setContactProcessingThreshold(0.0f);

	mSphere = sphere;
}

bool PhysicsSphere::getColliding() {
	btDiscreteDynamicsWorld *world = static_cast<PhysicsEngine *>(getWorld()->getPhysicsEngine())->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	for (U32 i = 0; i < manifolds; i++) {
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

glm::vec3 PhysicsSphere::getCollisionNormal(glm::vec3 &toiVelocity) {
	btDiscreteDynamicsWorld *world = static_cast<PhysicsEngine *>(getWorld()->getPhysicsEngine())->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	glm::vec3 total = glm::vec3(0.0f, 0.0f, 0.0f);
	toiVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	for (U32 i = 0; i < manifolds; i++) {
		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();

		if (obj1 == mActor || obj2 == mActor) {
			U32 contacts = manifold->getNumContacts();
			for (U32 j = 0; j < contacts; j++) {
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

bool PhysicsSphere::modifyContact(ContactCallbackInfo &info, bool isBody0) {
	//The interior with which we collided
	PhysicsInterior *inter = dynamic_cast<PhysicsInterior *>(isBody0 ? info.body1 : info.body0);
	if (inter == nullptr)
		return true;

	U32 index = isBody0 ? info.index1 : info.index0;

	//IO::printf("vel: %f %f %f\n", sphere->getLinearVelocity().x, sphere->getLinearVelocity().y, sphere->getLinearVelocity().z);
	info.point.m_impactVelocity = btConvert(getLinearVelocity());

	const PhysicsInterior::TriangleInfo &triangleInfo = inter->getTriangleInfo(index);

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
	}
	else if (stricmp(surfName.c_str(), "friction_high") == 0 ||
		stricmp(surfName.c_str(), "friction_high_shadow") == 0) {
		friction *= 2.5f;
	}

	info.point.m_combinedFriction *= friction;
	info.point.m_combinedRollingFriction *= friction;
	return true;
}

void PhysicsSphere::notifyContact(ContactCallbackInfo &info, bool isBody0) {
	//The interior with which we collided
	PhysicsInterior *inter = dynamic_cast<PhysicsInterior *>(isBody0 ? info.body1 : info.body0);
	if (inter == nullptr) {
		PhysicsSphere *sphere = dynamic_cast<PhysicsSphere *>(isBody0 ? info.body1 : info.body0);
		if (sphere != nullptr) {
			btVector3 impulse = info.point.m_normalWorldOnB * info.point.m_appliedImpulse;
			IO::printf("Sphere push: %f %f %f\n", impulse.x(), impulse.y(), impulse.z());
		}
		return;
	}

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
	const PhysicsInterior::TriangleInfo &triangleInfo = inter->getTriangleInfo(index);
	const DIF::Interior &dint = inter->getInterior()->getInterior(); //Encapsulation to the rescue

	glm::vec3 collisionPoint = btConvert(info.point.m_positionWorldOnB);

	//Triangle info from the interior
	TriangleF triangle(dint.point[triangleInfo.vertex[0]], dint.point[triangleInfo.vertex[1]], dint.point[triangleInfo.vertex[2]]);

	//We need to remove points if this isn't an edge collision
	if (triangle.isPointInside(collisionPoint) && !triangle.isPointOnEdge(collisionPoint)) {

		//Test all other manifold points
		btDispatcher *dispatcher = static_cast<PhysicsEngine*>(getWorld()->getPhysicsEngine())->getWorld()->getCollisionWorld()->getDispatcher();
		for (S32 i = 0; i < dispatcher->getNumManifolds(); i++) {
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
					for (S32 j = 0; j < manifold->getNumContacts(); j++) {
						btManifoldPoint &point = manifold->getContactPoint(j);

						//Access to index
						U32 pointIndex = (pointIsBody0 ? point.m_index1 : point.m_index0);

						//See if their point qualifies
						const PhysicsInterior::TriangleInfo &pointInfo = inter->getTriangleInfo(pointIndex);

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
			for (auto it = toRemove.rbegin(); it != toRemove.rend(); it++) {
				manifold->removeContactPoint(*it);
			}
		}
	}

	//How steep is the wall?
	F32 wallDot = info.point.m_normalWorldOnB.dot(btVector3(0, 0, 1));

	if ((wallDot * wallDot) < 0.0001f) {
		//		F32 appliedForce = sqrtf(glm::length(glm::proj(btConvert(info.point.m_impactVelocity), btConvert(info.point.m_normalWorldOnB))) * info.point.m_combinedFriction);
		//		IO::printf("Wall contact applied force %f\n", appliedForce);
		//Wall hit of some sort
		//		applyImpulse(glm::vec3(0, 0, appliedForce), glm::vec3(0, 0, 0));
	}
}

F32 PhysicsSphere::getRadius() {
	return static_cast<btSphereShape *>(mActor->getCollisionShape())->getRadius();
}

void PhysicsSphere::setRadius(const F32 &radius) {
	static_cast<btSphereShape *>(mActor->getCollisionShape())->setUnscaledRadius(radius);
}
