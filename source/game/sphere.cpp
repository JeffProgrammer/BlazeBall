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

#include "game/sphere.h"
#include "base/math.h"
#include "base/io.h"
#include "render/scene.h"
#include "physics/physicsSphere.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(Point3F origin, F32 radius) : radius(radius), renderBuffer(0), maxAngVel(1000.0f), material(NULL) {
	mActor = PhysicsEngine::getEngine()->createSphere(radius);
	mActor->setPosition(origin);
	mActor->setMass(1.0f);
	PhysicsEngine::getEngine()->addBody(mActor);
}

void Sphere::generate() {
	S32 segments2 = segments / 2;
	S32 slices2 = slices / 2;

	constexpr S32 size = segments * slices * 2;
	Vertex points[size];
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

void Sphere::applyTorque(const Point3F &torque) {
	mActor->applyTorque(torque);
}

void Sphere::applyImpulse(const Point3F &force, const Point3F &origin) {
	mActor->applyImpulse(force, origin);
}

void Sphere::applyForce(const Point3F &force, const Point3F &origin) {
	mActor->applyForce(force, origin);
}

bool Sphere::getColliding() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getColliding();
}

Point3F Sphere::getCollisionNormal() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getCollisionNormal();
}

const Point3F Sphere::getPosition() {
	return mActor->getPosition();
}

const AngAxisF Sphere::getRotation() {
	return mActor->getRotation();
}

void Sphere::setPosition(const Point3F &pos) {
	mActor->setPosition(pos);
}

void Sphere::setVelocity(const Point3F &vel) {
    mActor->setVelocity(vel);
}

void Sphere::setAngularVelocity(const Point3F &vel) {
    mActor->setAngularVelocity(vel);
}

void Sphere::updateCamera(const Movement &movement) {
	if (movement.pitchUp) cameraPitch -= Scene::keyCameraSpeed;
	if (movement.pitchDown) cameraPitch += Scene::keyCameraSpeed;
	if (movement.yawLeft) cameraYaw -= Scene::keyCameraSpeed;
	if (movement.yawRight) cameraYaw += Scene::keyCameraSpeed;

	cameraPitch += movement.pitch * Scene::cameraSpeed;
	cameraYaw += movement.yaw * Scene::cameraSpeed;
}

void Sphere::updateMove(const Movement &movement) {
	//Apply the camera yaw to a matrix so our rolling is based on the camera direction
	glm::mat4x4 delta = glm::mat4x4(1);
	delta = glm::rotate(delta, -cameraYaw, glm::vec3(0, 0, 1));

	//Convert the movement into a vector
	Point2F move = Point2F();
	if (movement.forward) move.x --;
	if (movement.backward) move.x ++;
	if (movement.left) move.y --;
	if (movement.right) move.y ++;

	//Torque is based on the movement and yaw
	glm::vec3 torque = glm::vec3(glm::translate(delta, glm::vec3(move.x, move.y, 0))[3]);

	//Multiplied by 2.5 (magic number alert)
	applyTorque(Point3F(torque.x, torque.y, torque.z) * 2.5);

	//If we are colliding with the ground, we have the chance to jump
	if (getColliding()) {
		//Make sure we're not trying to jump off a wall. Anything with a dot product > 0.1 is considered "not a wall"
		Point3F normal = getCollisionNormal();
		if (movement.jump && normal.dot(Point3F(0, 0, 1)) > 0.1) {
			//Jump takes the average of the collision normal and the up vector to provide a mostly upwards
			// jump but still taking the surface into account.
			applyImpulse((normal + Point3F(0, 0, 1)) / 2.f * 7.5f, Point3F(0, 0, 0));
		}
	} else {
		//If we're not touching the ground, apply slight air movement.
		applyForce(Point3F(torque.y, -torque.x, torque.z) * 5.f, Point3F(0, 0, 0));
	}
}

void Sphere::getCameraPosition(glm::mat4x4 &mat) {
	//Reset the matrix
	mat = glm::mat4x4(1);

	//Offset from marble; this is rotated by pitch and yaw
	mat = glm::translate(mat, glm::vec3(0, 2.5, 0));

	//Rotate camera around the marble, also rotating the offset above
	mat = glm::rotate(mat, cameraPitch, glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, cameraYaw, glm::vec3(0, 0, 1));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	Point3F pos = getPosition();
	mat = glm::translate(mat, glm::vec3(-pos.x, -pos.y, -pos.z));
}
