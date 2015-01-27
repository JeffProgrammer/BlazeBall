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

#include "sphere.h"
#include "math.h"
#include "io.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(Point3F origin, F32 radius) : origin(origin), radius(radius), renderBuffer(0), maxAngVel(1000.0f) {
	physx::PxMaterial *material = Physics::getPhysics()->getPxPhysics()->createMaterial(1.1f, 0.9f, 0.8f);
	actor = physx::PxCreateDynamic(*Physics::getPhysics()->getPxPhysics(), physx::PxTransform(origin.x, origin.y, origin.z), physx::PxSphereGeometry(radius), *material, 1.0f);
	physx::PxRigidBody *rigid = actor->is<physx::PxRigidBody>();
	if (rigid)
		physx::PxRigidBodyExt::updateMassAndInertia(*rigid, 1);

	Physics::getPhysics()->addActor(actor);
//	//Motion state and shape
//	btMotionState *state = new btDefaultMotionState();
//	btCollisionShape *shape = new btSphereShape(radius);
//
//	//Need this otherwise forces won't work!
//	btVector3 fallInertia = btVector3(0.f, 0.f, 0.f);
//	shape->calculateLocalInertia(1.0f, fallInertia);
//	shape->setMargin(0.01f);
//
//	//Update position
//	btTransform transform;
//	transform.setIdentity();
//	transform.setOrigin(btConvert(origin));
//
//	state->setWorldTransform(transform);
//
//	//Construction info
//	btRigidBody::btRigidBodyConstructionInfo info(1, state, shape, fallInertia);
//	info.m_linearDamping = 0.3f;
//	info.m_angularDamping = 0.4f;
//	info.m_restitution = 0.8f;
//	info.m_friction = 1.1f;
//	info.m_rollingFriction = 0.4f;
//
//	//Create the actor and add it to the scene
//	actor = new btRigidBody(info);
//	actor->setActivationState(DISABLE_DEACTIVATION);
//	actor->setCcdMotionThreshold(1e-3);
//	actor->setCcdSweptSphereRadius(radius / 10.0f);
//	actor->setAnisotropicFriction(shape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
//	Physics::getPhysics()->addRigidBody(actor);
}

void Sphere::generate() {
	S32 segments2 = segments / 2;
	S32 slices2 = slices / 2;

	Vertex *points = new Vertex[segments * slices * 2];
	U32 point = 0;

	for (S32 y = -slices2; y < slices2; y ++) {
		float cosy = cos(y * step);
		float cosy1 = cos((y + 1) * step);
		float siny = sin(y * step);
		float siny1 = sin((y + 1) * step);

		for (S32 i = -segments2; i < segments2; i ++) {
			float cosi = cos(i * step);
			float sini = sin(i * step);

			//Math not invented by me
			Point3F point0 = Point3F(radius * cosi * cosy, radius * siny, radius * sini * cosy);
			Point3F point1 = Point3F(radius * cosi * cosy1, radius * siny1, radius * sini * cosy1);
			Point4F color0 = Point4F(fabs(point0.x), fabs(point0.y), fabs(point0.z), radius).normalize();
			Point4F color1 = Point4F(fabs(point1.x), fabs(point1.y), fabs(point1.z), radius).normalize();

			color0 /= color0.z;
			color1 /= color1.z;

			Point2F uv0 = Point2F((F32)i / (F32)segments2, (F32)y / (F32)slices2);
			Point2F uv1 = Point2F((F32)i / (F32)segments2, (F32)(y + 1) / (F32)slices2);

			Point3F tangent0 = point0.cross(Point3F(0, 0, 1)).normalize();
			Point3F tangent1 = point1.cross(Point3F(0, 0, 1)).normalize();
			Point3F bitangent0 = point0.cross(tangent0).normalize();
			Point3F bitangent1 = point0.cross(tangent0).normalize();

			points[point].point = point0;
			points[point].uv = uv0;
			points[point].normal = point0;
			points[point].tangent = tangent0;
			points[point].bitangent = bitangent0;
			point ++;
			points[point].point = point1;
			points[point].uv = uv1;
			points[point].normal = point1;
			points[point].tangent = tangent1;
			points[point].bitangent = bitangent1;
			point ++;
		}
	}

	//Generate a buffer
	glGenBuffers(1, &renderBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * point, points, GL_STATIC_DRAW);

	delete [] points;
}

void Sphere::render(ColorF color) {
	if (!renderBuffer)
		generate();

	if (material) {
		material->activate();
	}
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, renderBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, point));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, segments * slices * 2);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	if (material) {
		material->deactivate();
	}
}

void Sphere::applyTorque(Point3F torque) {
	physx::PxRigidDynamic *dynamic = actor->is<physx::PxRigidDynamic>();

	if (dynamic)
		dynamic->addTorque(pxConvert<Point3F, physx::PxVec3>(torque));

//	actor->applyTorqueImpulse(btConvert(torque));
//	Point3F ang = btConvert(actor->getAngularVelocity());
//
//	if (ang.length() > maxAngVel)
//		ang = ang.normalize(maxAngVel);
//	
//	actor->setAngularVelocity(btConvert(ang));
}

void Sphere::applyImpulse(Point3F force, Point3F origin) {
	physx::PxRigidDynamic *dynamic = actor->is<physx::PxRigidDynamic>();

	if (dynamic)
		physx::PxRigidBodyExt::addForceAtPos(*dynamic, pxConvert<Point3F, physx::PxVec3>(force), pxConvert<Point3F, physx::PxVec3>(origin));
}

void Sphere::applyForce(Point3F force, Point3F origin) {
	physx::PxRigidDynamic *dynamic = actor->is<physx::PxRigidDynamic>();

	if (dynamic)
		physx::PxRigidBodyExt::addForceAtPos(*dynamic, pxConvert<Point3F, physx::PxVec3>(force), pxConvert<Point3F, physx::PxVec3>(origin));
}

bool Sphere::colliding() {
//	btDiscreteDynamicsWorld *world = Physics::getPhysics()->getWorld();
//	U32 manifolds = world->getDispatcher()->getNumManifolds();
//
//	for (U32 i = 0; i < manifolds; i ++) {
//		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
//		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
//		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();
//
//		if (obj1 == actor || obj2 == actor) {
//			if (manifold->getNumContacts() > 0)
//				return true;
//		}
//	}

	return false;
}

Point3F Sphere::getCollisionNormal() {
//	btDiscreteDynamicsWorld *world = Physics::getPhysics()->getWorld();
//	U32 manifolds = world->getDispatcher()->getNumManifolds();
//
//	Point3F best = Point3F(0.0f, 0.0f, 0.0f);
//	F32 dot = 0;
//
//	for (U32 i = 0; i < manifolds; i ++) {
//		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
//		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
//		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();
//
//		if (obj1 == actor || obj2 == actor) {
//			U32 contacts = manifold->getNumContacts();
//			for (U32 j = 0; j < contacts; j ++) {
//				Point3F normal = btConvert(manifold->getContactPoint(j).m_normalWorldOnB);
//				if (obj2 == actor)
//					normal *= -1;
//				if (normal.dot(Point3F(0, 0, 1)) > dot) {
//					best = normal;
//					dot = normal.dot(Point3F(0, 0, 1));
//				}
//			}
//		}
//	}

	return Point3F(0,0,0); //best
}

Point3F Sphere::getPosition() {
	return pxConvert<physx::PxTransform, Point3F>(actor->getGlobalPose());
}

Point3F Sphere::getPosition() const {
	return pxConvert<physx::PxTransform, Point3F>(actor->getGlobalPose());
}

AngAxisF Sphere::getRotation() {
	return AngAxisF(pxConvert<physx::PxTransform, QuatF>(actor->getGlobalPose()));
}

AngAxisF Sphere::getRotation() const {
	return AngAxisF(pxConvert<physx::PxTransform, QuatF>(actor->getGlobalPose()));
}

void Sphere::setPosition(Point3F pos) {
//	btTransform trans;
//	trans.setOrigin(btConvert(pos));
//	actor->getMotionState()->setWorldTransform(trans);
//	actor->setWorldTransform(trans);
//	actor->setLinearVelocity(btConvert(Point3F(0, 0, 0)));
}

void Sphere::setPosition(const Point3F pos) const {
//	btTransform trans;
//	trans.setOrigin(btConvert(pos));
//	actor->getMotionState()->setWorldTransform(trans);
//	actor->setWorldTransform(trans);
//	actor->setLinearVelocity(btConvert(Point3F(0, 0, 0)));
}
