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
#include <OpenGL/gl.h>

Sphere::Sphere(Point3F origin, F32 radius) : origin(origin), radius(radius) {
	generate();

	btMotionState *state = new btDefaultMotionState();
	btCollisionShape *shape = new btSphereShape(radius);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	state->setWorldTransform(transform);

	actor = new btRigidBody(1, state, shape);
	Physics::getPhysics()->addRigidBody(actor);
}

void Sphere::generate() {
	S32 segments2 = segments / 2;
	S32 slices2 = slices / 2;

	for (S32 y = -slices2; y < slices2; y ++) {
		float cosy = cos(y * step);
		float cosy1 = cos((y + 1) * step);
		float siny = sin(y * step);
		float siny1 = sin((y + 1) * step);

		for (S32 i = -segments2; i < segments2; i ++) {
			float cosi = cos(i * step);
			float sini = sin(i * step);

			geometry.push_back(Point3F(radius * cosi * cosy, radius * siny, radius * sini * cosy));
			geometry.push_back(Point3F(radius * cosi * cosy1, radius * siny1, radius * sini * cosy1));
		}
	}
}

void Sphere::render(ColorF color) {
	glPushMatrix();
	btTransform trans;
	actor->getMotionState()->getWorldTransform(trans);
	btVector3 position = trans.getOrigin();
	glTranslatef(position.x(), position.y(), position.z());
	glEnable(GL_COLOR_MATERIAL);
	glColor4fv(&color.red);
	glBegin(GL_TRIANGLE_STRIP);
	for (Point3F point : geometry) {
		glNormal3fv(&point.x);
		glVertex3fv(&point.x);
	}
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}
