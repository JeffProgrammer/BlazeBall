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

#ifndef types_h
#define types_h

#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

#include <dif/base/types.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

//Base types (names stolen from TGE because lazy)
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

typedef signed char      S8;
typedef signed short     S16;
typedef signed int       S32;
typedef signed long long S64;

typedef float F32;
typedef double F64;

#include "base/color.h"

typedef Color<U8>  ColorI;
typedef Color<F32> ColorF;

//More names stolen from TGE

class AngAxisF {
public:
	glm::vec3 axis;
	F32 angle;

	AngAxisF(const glm::vec3 &axis, const F32 &angle) : axis(axis), angle(angle) {};
	AngAxisF(const F32 &angle, const glm::vec3 &axis) : axis(axis), angle(angle) {};
	AngAxisF(const glm::quat &quat);

	operator glm::quat();
	operator glm::quat() const;
};

inline AngAxisF::AngAxisF(const glm::quat &quat) {
	angle = acosf(quat.w);
	F32 half = sqrtf(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
	if (half != 0.0f) {
		axis = glm::vec3(quat.x / half, quat.y / half, quat.z / half);
	} else {
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
}

inline AngAxisF::operator glm::quat() {
	F32 sin2 = sinf(angle / 2);
	return glm::quat(cosf(angle / 2), axis.x * sin2, axis.y * sin2, axis.z * sin2);
}

inline AngAxisF::operator glm::quat() const {
	F32 sin2 = sinf(angle / 2);
	return glm::quat(cosf(angle / 2), axis.x * sin2, axis.y * sin2, axis.z * sin2);
}

class PlaneF {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 d;

	PlaneF(const DIF::PlaneF plane) : x(plane.x), y(plane.y), z(plane.z), d(plane.d) {}
};

class BoxF {
public:
	F32 minX;
	F32 minY;
	F32 minZ;
	F32 maxX;
	F32 maxY;
	F32 maxZ;

	inline glm::vec3 getMin() const {
		return glm::vec3(minX, minY, minZ);
	}
	inline glm::vec3 getMax() const {
		return glm::vec3(maxX, maxY, maxZ);
	}
	inline glm::vec3 getCenter() const {
		return (getMax() + getMin()) / 2.0f;
	}
};

class TriangleF {
public:
	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;
};

class TetrahetronF {
public:
	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
};

#include "base/ray.h"
typedef Ray<F32> RayF;

struct Vertex {
	glm::vec3 point;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Triangle {
	Vertex verts[3];
};

#endif
