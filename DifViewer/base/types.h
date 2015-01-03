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

#ifndef types_h
#define types_h

#include <stdbool.h>

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

//Pointer types
typedef U8 * String;

#include "point2.h"
#include "point3.h"
#include "point4.h"
#include "color.h"

typedef Point2<U16> Point2I;
typedef Point2<F32> Point2F;
typedef Point3<U32> Point3I;
typedef Point3<F32> Point3F;
typedef Point3<F64> Point3D;
typedef Point4<F32> Point4F;

typedef Color<U8>  ColorI;
typedef Color<F32> ColorF;

//More names stolen from TGE

class AngAxisF {
public:
	Point3F axis;
	F32 angle;

	AngAxisF(Point3F axis, F32 angle) : axis(axis), angle(angle) {};
	AngAxisF(F32 angle, Point3F axis) : axis(axis), angle(angle) {};
};

class QuatF {
public:
	F32 w;
	F32 x;
	F32 y;
	F32 z;
};

class PlaneF {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 d;
};

class BoxF {
public:
	F32 minX;
	F32 minY;
	F32 minZ;
	F32 maxX;
	F32 maxY;
	F32 maxZ;
};

class SphereF {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 radius;
};

class Dictionary {
public:
	U32 size;
	String *names;
	String *values;
};

class PNG {
public:
	U32 size;
	U8 *data;
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

#include "ray.h"
typedef Ray<F32> RayF;

#endif
