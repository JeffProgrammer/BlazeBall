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
#include "game/movement.h"
#include <glm/matrix.hpp>

class Sphere : public GameObject {
protected:
	std::vector<glm::vec3> geometry;
public:
	PhysicsBody *mActor;
	F32 radius;
	F32 maxAngVel;
	Material *material;

	GLuint renderBuffer;

	F32 cameraYaw;
	F32 cameraPitch;
private:
	void generate();
	const static U32 segments = 36;
	const static U32 slices = 18;
	F32 step;

public:
	Sphere(glm::vec3 origin, F32 radius);

	void render();
	const glm::vec3 getPosition();
	const glm::quat getRotation();

	void setPosition(const glm::vec3 &pos);

	void setMaterial(Material *material) {
		this->material = material;
	}

	void applyTorque(const glm::vec3 &torque);
	void applyImpulse(const glm::vec3 &force, const glm::vec3 &origin);
	void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	bool getColliding();
	glm::vec3 getCollisionNormal();
    
    void setVelocity(const glm::vec3 &vel);
    void setAngularVelocity(const glm::vec3 &vel);

	virtual void updateCamera(const Movement &movement);
	virtual void updateMove(const Movement &movement);
	virtual void getCameraPosition(glm::mat4x4 &mat);
};

#endif