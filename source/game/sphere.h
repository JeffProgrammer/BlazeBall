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

#ifndef sphere_h
#define sphere_h

#include <stdio.h>
#include <vector>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif
#include "gameObject.h"
#include "base/types.h"
#include "physics/physicsBody.h"
#include "render/material.h"
#include "graphics/vertexBufferObject.h"
#include "graphics/vertexInputLayout.h"
#include "game/movement.h"
#include <glm/matrix.hpp>

class Sphere : public GameObject {
	const F32 AppliedAcceleration = 150.f; //Maximum, lowers when reaching max roll velocity
	const F32 MaxRollVelocity     = 15.f; //In one direction (diagonal-supported)
	const F32 MaxAirSpinVelocity  = 50.f; //Same as above but for angular, only when falling
	const F32 JumpImpulse         = 7.5f; //Per jump, magnitude of the vector
	const F32 AirAcceleration     = 5.f; //Applied every tick in the air
	const F32 LinearRollDamping   = 0.025f; // 97.5% of original

protected:
	std::vector<glm::vec3> geometry;
public:
	PhysicsBody *mActor;
	F32 radius;
	F32 maxAngVel;
	Material *material;

	static VertexBufferObject *sVBO;
	VertexInputLayout *mInputLayout;
	bool firstDraw;

	F32 cameraYaw;
	F32 cameraPitch;

	U32 jumpTicks;
private:
	void generate();

	const U32 segments = 36;
	const U32 slices = 18;

	const F32 cameraSpeed = 0.3f;
	const F32 keyCameraSpeed = 3.f;

public:
	Sphere(glm::vec3 origin, F32 radius);
	virtual ~Sphere();

	virtual void loadMatrix(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, glm::mat4 &modelMatrix);
	virtual void render();

	virtual glm::vec3 getPosition();
	virtual glm::quat getRotation();

	virtual void setPosition(const glm::vec3 &pos);
	virtual void setRotation(const glm::quat &rot);

	F32 getRadius();
	void setRadius(const F32 &radius);

	void setMaterial(Material *material) {
		this->material = material;
	}

	void applyTorque(const glm::vec3 &torque);
	void applyImpulse(const glm::vec3 &force, const glm::vec3 &origin);
	void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	bool getColliding();
	glm::vec3 getCollisionNormal();
    
	glm::vec3 getVelocity();
	glm::vec3 getAngularVelocity();

	void setVelocity(const glm::vec3 &vel);
    void setAngularVelocity(const glm::vec3 &vel);

	virtual void updateCamera(const Movement &movement, const F64 &deltaMS);
	virtual void updateMove(const Movement &movement, const F64 &deltaMS);
	virtual void getCameraPosition(glm::mat4x4 &mat);

	virtual void updateTick(const F64 &deltaMS);

	OBJECT_CONSTRUCTOR(Sphere, (glm::vec3(0.0f), 0.2f)) {
		if (args.Length() >= 1) {
			object->setPosition(UNWRAP_EXTERN(glm::vec3, args[0]));
		}
		if (args.Length() >= 2) {
			object->setRadius(static_cast<F32>(args[1]->ToNumber()->Value()));
		}
	}
};

#endif