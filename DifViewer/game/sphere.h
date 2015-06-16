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

#ifdef BUILD_PHYSICS
#ifndef sphere_h
#define sphere_h

#include <stdio.h>
#include <vector>
#include <OpenGL/gl.h>

#include "types.h"
#include "interior.h"
#include "physicsBody.h"
#include "material.h"

class Sphere {
protected:
	std::vector<Point3F> geometry;
public:
	PhysicsBody *mActor;
	Point3F origin;
	F32 radius;
	F32 maxAngVel;
	Material *material;

	GLuint renderBuffer;
private:
	void generate();
	const static U32 segments = 36;
	const static U32 slices = 18;
	constexpr const static F32 step = (M_PI * 2.0f / segments);

public:
	Sphere(Point3F origin, F32 radius);

	void render(ColorF color);
	const Point3F getPosition();
	const AngAxisF getRotation();

	void setPosition(const Point3F &pos);

	void setMaterial(Material *material) {
		this->material = material;
	}

	void applyTorque(const Point3F &torque);
	void applyImpulse(const Point3F &force, const Point3F &origin);
	void applyForce(const Point3F &force, const Point3F &origin);

	bool getColliding();
	Point3F getCollisionNormal();
    
    void setVelocity(const Point3F &vel);
    void setAngularVelocity(const Point3F &vel);
};

#endif
#endif
