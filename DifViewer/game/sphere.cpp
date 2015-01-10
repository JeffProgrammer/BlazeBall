//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
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

Sphere::Sphere(Point3F origin, F32 radius) : origin(origin), radius(radius), displayList(0) {
	//Motion state and shape
	btMotionState *state = new btDefaultMotionState();
	btCollisionShape *shape = new btSphereShape(radius);

	//Need this otherwise forces won't work!
	btVector3 fallInertia = btVector3(0.f, 0.f, 0.f);
	shape->calculateLocalInertia(1.0f, fallInertia);

	//Update position
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btConvert(origin));

	state->setWorldTransform(transform);

	//Construction info
	btRigidBody::btRigidBodyConstructionInfo info(1, state, shape, fallInertia);
	info.m_linearDamping = 0.3f;
	info.m_angularDamping = 0.4f;
	info.m_restitution = 0.7f;
	info.m_friction = 1.1f;
	info.m_rollingFriction = 1.1f;

	//Create the actor and add it to the scene
	actor = new btRigidBody(info);
	actor->setActivationState(DISABLE_DEACTIVATION);
	actor->setCcdMotionThreshold(1e-7);
	actor->setCcdSweptSphereRadius(radius / 2.0f);
	Physics::getPhysics()->addRigidBody(actor);
}

void Sphere::generate() {
	S32 segments2 = segments / 2;
	S32 slices2 = slices / 2;

	//Use a displaylist for superfast drawing
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE_AND_EXECUTE);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_TRIANGLE_STRIP);

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

			glColor4fv(&color0.w);
			glNormal3fv(&point0.x);
			glVertex3fv(&point0.x);
			glColor4fv(&color1.w);
			glNormal3fv(&point1.x);
			glVertex3fv(&point1.x);
		}
	}
	glEnd();
	//Reset color so we don't pollute the scene
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEndList();
}

void Sphere::render(ColorF color) {
	glPushMatrix();
	btTransform trans;
	Point3F pos = getPosition();
	AngAxisF rot = getRotation();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.angle * (360.f / (M_PI * 2.f)), rot.axis.x, rot.axis.y, rot.axis.z);

	if (displayList == 0) {
		generate();
	} else {
		glCallList(displayList);
	}

	glPopMatrix();
}

void Sphere::applyTorque(Point3F torque) {
	actor->applyTorqueImpulse(btConvert(torque));
}

void Sphere::applyImpulse(Point3F force, Point3F origin) {
	actor->applyImpulse(btConvert(force), btConvert(origin));
}

void Sphere::applyForce(Point3F force, Point3F origin) {
	actor->applyForce(btConvert(force), btConvert(origin));
}

bool Sphere::colliding() {
	btDiscreteDynamicsWorld *world = Physics::getPhysics()->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	for (U32 i = 0; i < manifolds; i ++) {
		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();

		if (obj1 == actor || obj2 == actor) {
			if (manifold->getNumContacts() > 0)
				return true;
		}
	}

	return false;
}

Point3F Sphere::getCollisionNormal() {
	btDiscreteDynamicsWorld *world = Physics::getPhysics()->getWorld();
	U32 manifolds = world->getDispatcher()->getNumManifolds();

	Point3F best = Point3F(0.0f, 0.0f, 0.0f);
	F32 dot = 0;

	for (U32 i = 0; i < manifolds; i ++) {
		btPersistentManifold *manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject *obj1 = (btCollisionObject *)manifold->getBody0();
		btCollisionObject *obj2 = (btCollisionObject *)manifold->getBody1();

		if (obj1 == actor || obj2 == actor) {
			U32 contacts = manifold->getNumContacts();
			for (U32 j = 0; j < contacts; j ++) {
				Point3F normal = btConvert(manifold->getContactPoint(j).m_normalWorldOnB);
				if (obj2 == actor)
					normal *= -1;
				if (normal.dot(Point3F(0, 0, 1)) > dot) {
					best = normal;
					dot = normal.dot(Point3F(0, 0, 1));
				}
			}
		}
	}

	return best;
}

Point3F Sphere::getPosition() {
	btTransform trans;
	actor->getMotionState()->getWorldTransform(trans);
	return btConvert(trans.getOrigin());
}

Point3F Sphere::getPosition() const {
	btTransform trans;
	actor->getMotionState()->getWorldTransform(trans);
	return btConvert(trans.getOrigin());
}

AngAxisF Sphere::getRotation() {
	btTransform trans;
	actor->getMotionState()->getWorldTransform(trans);
	return btConvert(trans.getRotation());
}

AngAxisF Sphere::getRotation() const {
	btTransform trans;
	actor->getMotionState()->getWorldTransform(trans);
	return btConvert(trans.getRotation());
}


