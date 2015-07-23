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

#include "base/point2.h"
#include "base/point3.h"
#include "base/point4.h"
#include "base/color.h"

typedef Point2<S16> Point2I;
typedef Point2<F32> Point2F;
typedef Point3<S32> Point3I;
typedef Point3<F32> Point3F;
typedef Point3<F64> Point3D;
typedef Point4<F32> Point4F;

typedef Color<U8>  ColorI;
typedef Color<F32> ColorF;

//More names stolen from TGE

class QuatF;

class AngAxisF {
public:
	Point3F axis;
	F32 angle;

	AngAxisF(const Point3F &axis, const F32 &angle) : axis(axis), angle(angle) {};
	AngAxisF(const F32 &angle, const Point3F &axis) : axis(axis), angle(angle) {};
	AngAxisF(const QuatF &quat);
};

class QuatF {
public:
	F32 w;
	F32 x;
	F32 y;
	F32 z;

	QuatF() : x(0), y(0), z(0), w(0) {};
	QuatF(const F32 &x, const F32 &y, const F32 &z, const F32 &w) : x(x), y(y), z(z), w(w) {};
	QuatF(const AngAxisF &ang);
};

inline AngAxisF::AngAxisF(const QuatF &quat) {
	angle = acosf(quat.w);
	F32 half = sqrtf(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
	if (half != 0.0f) {
		axis = Point3F(quat.x / half, quat.y / half, quat.z / half);
	} else {
		axis = Point3F(1.0f, 0.0f, 0.0f);
	}
}

inline QuatF::QuatF(const AngAxisF &ang) {
	F32 sin2 = sinf(ang.angle / 2);
	w = cosf(ang.angle / 2);
	x = ang.axis.x * sin2;
	y = ang.axis.y * sin2;
	z = ang.axis.z * sin2;
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

	inline Point3F getMin() const {
		return Point3F(minX, minY, minZ);
	}
	inline Point3F getMax() const {
		return Point3F(maxX, maxY, maxZ);
	}
	inline Point3F getCenter() const {
		return (getMax() + getMin()) / 2;
	}
};

class TriangleF {
public:
	Point3F point0;
	Point3F point1;
	Point3F point2;
};

class TetrahetronF {
public:
	Point3F point0;
	Point3F point1;
	Point3F point2;
	Point3F point3;
};

class MatrixF {
public:
	F32 m[16];
};

#include "base/ray.h"
typedef Ray<F32> RayF;

struct Vertex {
	Point3F point;
	Point2F uv;
	Point3F normal;
	Point3F tangent;
	Point3F bitangent;
};

struct Triangle {
	Vertex verts[3];
};

#endif
