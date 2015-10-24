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

#include "game/sphere.h"
#include "base/math.h"
#include "base/io.h"
#include "render/scene.h"
#include "physics/physicsSphere.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint gSphereVBO = 0;

Sphere::Sphere(glm::vec3 origin, F32 radius) : GameObject(), radius(radius), maxAngVel(1000.0f), material(nullptr) {
	mActor = PhysicsEngine::getEngine()->createSphere(radius);
	mActor->setPosition(origin);
	mActor->setMass(1.0f);
	PhysicsEngine::getEngine()->addBody(mActor);
	
	firstDraw = false;

	cameraYaw = 0.0f;
	cameraPitch = 0.0f;
}

Sphere::~Sphere() {
	// todo: remove body from physics engine
}

void Sphere::generate() {
	if (gSphereVBO)
		return;

	F32 step = (glm::pi<F32>() * 2.0f / segments);

	S32 segments2 = segments / 2;
	S32 slices2 = slices / 2;

	S32 size = segments * slices * 2;
	Vertex *points = new Vertex[size];

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
			glm::vec3 point0 = glm::vec3(cosi * cosy, siny, sini * cosy);
			glm::vec3 point1 = glm::vec3(cosi * cosy1, siny1, sini * cosy1);

			glm::vec2 uv0 = glm::vec2((F32)i / (F32)segments2, (F32)y / (F32)slices2);
			glm::vec2 uv1 = glm::vec2((F32)i / (F32)segments2, (F32)(y + 1) / (F32)slices2);

			glm::vec3 tangent0 = glm::normalize(glm::cross(point0, glm::vec3(0, 0, 1)));
			glm::vec3 tangent1 = glm::normalize(glm::cross(point1, glm::vec3(0, 0, 1)));
			glm::vec3 bitangent0 = glm::normalize(glm::cross(point0, tangent0));
			glm::vec3 bitangent1 = glm::normalize(glm::cross(point1, tangent1));

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
	glGenBuffers(1, &gSphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gSphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * point, &points[0], GL_STATIC_DRAW);
}

void Sphere::loadMatrix(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, glm::mat4 &modelMatrix) {
	GameObject::loadMatrix(projectionMatrix, viewMatrix, modelMatrix);

	modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));
}

void Sphere::render(const RenderInfo &info) {
	if (!firstDraw) {
		generate();
		firstDraw = true;
	}

	if (!material)
		return;

	Shader *shader = material->getShader();
	material->activate();
	info.loadShader(shader);

	glUniform1f(shader->getUniformLocation("reflectivity"), 1.f);

	glBindBuffer(GL_ARRAY_BUFFER, gSphereVBO);
	// enable attributes

	shader->enableAttribute("vertexPosition",  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	shader->enableAttribute("vertexUV",        2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	shader->enableAttribute("vertexNormal",    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	shader->enableAttribute("vertexTangent",   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	shader->enableAttribute("vertexBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, segments * slices * 2);

	shader->disableAttribute("vertexPosition");
	shader->disableAttribute("vertexUV");
	shader->disableAttribute("vertexNormal");
	shader->disableAttribute("vertexTangent");
	shader->disableAttribute("vertexBitangent");

	material->deactivate();
}

void Sphere::applyTorque(const glm::vec3 &torque) {
	mActor->applyTorque(torque);
}

void Sphere::applyImpulse(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyImpulse(force, origin);
}

void Sphere::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyForce(force, origin);
}

bool Sphere::getColliding() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getColliding();
}

glm::vec3 Sphere::getCollisionNormal() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getCollisionNormal();
}

glm::vec3 Sphere::getPosition() {
	return mActor->getPosition();
}

glm::quat Sphere::getRotation() {
	return mActor->getRotation();
}

glm::vec3 Sphere::getVelocity() {
	return mActor->getLinearVelocity();
}

glm::vec3 Sphere::getAngularVelocity() {
	return mActor->getAngularVelocity();
}

void Sphere::setPosition(const glm::vec3 &pos) {
	mActor->setPosition(pos);
}

void Sphere::setRotation(const glm::quat &rot) {
	mActor->setRotation(rot);
}

void Sphere::setVelocity(const glm::vec3 &vel) {
    mActor->setVelocity(vel);
}

void Sphere::setAngularVelocity(const glm::vec3 &vel) {
    mActor->setAngularVelocity(vel);
}

F32 Sphere::getRadius() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getRadius();
}

void Sphere::setRadius(const F32 &radius) {
	this->radius = radius;
	dynamic_cast<PhysicsSphere *>(mActor)->setRadius(radius);
}

void Sphere::updateCamera(const Movement &movement, const F64 &deltaMS) {
	if (movement.pitchUp) cameraPitch -= keyCameraSpeed;
	if (movement.pitchDown) cameraPitch += keyCameraSpeed;
	if (movement.yawLeft) cameraYaw -= keyCameraSpeed;
	if (movement.yawRight) cameraYaw += keyCameraSpeed;

	cameraPitch += movement.pitch * cameraSpeed;
	cameraYaw += movement.yaw * cameraSpeed;
}

void Sphere::updateMove(const Movement &movement, const F64 &deltaMS) {
	//Apply the camera yaw to a matrix so our rolling is based on the camera direction
	glm::mat4x4 delta = glm::mat4x4(1);
	delta = glm::rotate(delta, -cameraYaw, glm::vec3(0, 0, 1));

	F32 deltaSeconds = static_cast<F32>(deltaMS) / 1000.0f;

	//Convert the movement into a vector
	glm::vec3 move = glm::vec3();
	if (movement.forward) move.y ++;
	if (movement.backward) move.y --;
	if (movement.left) move.x --;
	if (movement.right) move.x ++;

	//Linear velocity relative to camera yaw (for capping)
	glm::vec3 linRel = glm::vec3(glm::translate(glm::inverse(delta), mActor->getLinearVelocity())[3]);
	glm::vec3 angRel = glm::cross(glm::vec3(glm::translate(glm::inverse(delta), mActor->getAngularVelocity())[3]), glm::vec3(0, 0, 1));
	glm::vec3 torque = move * AppliedAcceleration * deltaSeconds;

	if (getColliding()) {
		//Don't let us go faster than the max velocity in any direction.
		if (torque.x > 0 && torque.x + linRel.x >  MaxRollVelocity) torque.x = glm::max(0.f,  MaxRollVelocity - linRel.x);
		if (torque.y > 0 && torque.y + linRel.y >  MaxRollVelocity) torque.y = glm::max(0.f,  MaxRollVelocity - linRel.y);
		//Same for backwards
		if (torque.x < 0 && torque.x + linRel.x < -MaxRollVelocity) torque.x = glm::min(0.f, -MaxRollVelocity - linRel.x);
		if (torque.y < 0 && torque.y + linRel.y < -MaxRollVelocity) torque.y = glm::min(0.f, -MaxRollVelocity - linRel.y);
	} else {
		//Don't let us roll faster than the max air roll velocity
		if (torque.x > 0 && torque.x + angRel.x >  MaxAirSpinVelocity) torque.x = glm::max(0.0f,  MaxAirSpinVelocity - angRel.x);
		if (torque.y > 0 && torque.y + angRel.y >  MaxAirSpinVelocity) torque.y = glm::max(0.0f,  MaxAirSpinVelocity - angRel.y);
		if (torque.x < 0 && torque.x + angRel.x < -MaxAirSpinVelocity) torque.x = glm::min(0.0f, -MaxAirSpinVelocity - angRel.x);
		if (torque.y < 0 && torque.y + angRel.y < -MaxAirSpinVelocity) torque.y = glm::min(0.0f, -MaxAirSpinVelocity - angRel.y);
	}

//	printf("LR: %f %f\n", linRel.x, linRel.y);
//	printf("AR: %f %f\n", angRel.x, angRel.y);
//	printf("T:  %f %f\n", torque.x, torque.y);

	//Torque is based on the movement and yaw
	glm::vec3 torqueRel = glm::vec3(glm::translate(delta, torque)[3]);
	//Cross to convert 3d coordinates into torque
	applyTorque(glm::cross(glm::vec3(0, 0, 1), torqueRel));

	//If we are colliding with the ground, we have the chance to jump
	if (getColliding()) {
		if (jumpTicks == 0) {
			//Make sure we're not trying to jump off a wall. Anything with a dot product > 0.1 is considered "not a wall"
			glm::vec3 normal = getCollisionNormal();
			if (movement.jump && glm::dot(normal, glm::vec3(0, 0, 1)) > 0.1) {
				//Jump takes the average of the collision normal and the up vector to provide a mostly upwards
				// jump but still taking the surface into account.
				glm::vec3 jumpNormal = (normal + glm::vec3(0, 0, 1)) / 2.f;
				glm::vec3 vel = mActor->getLinearVelocity();

				glm::vec3 proj = jumpNormal * (glm::dot(vel, jumpNormal) * glm::dot(jumpNormal, jumpNormal));
				printf("jump normal: %f %f %f\nvel: %f %f %f\nproj: %f %f %f\n",
					   jumpNormal.x, jumpNormal.y, jumpNormal.z,
					   vel.x, vel.y, vel.z,
					   proj.x, proj.y, proj.z
					   );

				if (glm::length(proj) < JumpImpulse) {
					applyImpulse(jumpNormal * (JumpImpulse - glm::length(proj)), glm::vec3(0, 0, 0));
				}

				printf("Jump!\n");
			}
			jumpTicks = 2;
		} else {
			jumpTicks --;
		}
	} else {
		glm::vec3 moveRel = glm::vec3(glm::translate(delta, move)[3]);
		moveRel *= AirAcceleration * deltaSeconds;

		//If we're not touching the ground, apply slight air movement.
		applyImpulse(moveRel, glm::vec3(0, 0, 0));
	}
	//Crappy damping
	if (movement.forward + movement.backward + movement.left + movement.right == 0 && getColliding()) {
		F32 damping = 1.f - LinearRollDamping;
		mActor->setAngularVelocity(mActor->getAngularVelocity() * damping);
	}
}

void Sphere::getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos) {
	//Reset the matrix
	mat = glm::mat4x4(1);

	//Offset from marble; this is rotated by pitch and yaw
	mat = glm::translate(mat, glm::vec3(0, 2.5, 0));

	//Rotate camera around the marble, also rotating the offset above
	mat = glm::rotate(mat, cameraPitch, glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, cameraYaw, glm::vec3(0, 0, 1));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = glm::translate(mat, -getPosition());

	glm::vec3 rot = glm::mat3(glm::inverse(mat)) * glm::vec3(0, -2.5, 0);
	pos = getPosition() + rot;
}

void Sphere::updateTick(const F64 &deltaMS) {
	//Temporary OOB solution for now
	if (getPosition().z < -40) {
		setPosition(glm::vec3(0, 0, 60));
		setVelocity(glm::vec3(0, 0, 0));
		setAngularVelocity(glm::vec3(0, 0, 0));
	}
}