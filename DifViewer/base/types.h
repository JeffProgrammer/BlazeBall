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

//Pointer types
typedef U8 * String;

//More names stolen from TGE
typedef struct {
	U16 x; //Should be enough
	U16 y;
} Point2I;

typedef struct {
	F32 x;
	F32 y;
} Point2F;

typedef struct {
	U32 x;
	U32 y;
	U32 z;
} Point3I;

typedef struct {
	F32 x;
	F32 y;
	F32 z;
} Point3F;

typedef struct {
	F32 w;
	F32 x;
	F32 y;
	F32 z;
} QuatF;

typedef struct {
	F32 x;
	F32 y;
	F32 z;
	F32 d;
} PlaneF;

typedef struct {
	F32 minX;
	F32 minY;
	F32 minZ;
	F32 maxX;
	F32 maxY;
	F32 maxZ;
} BoxF;

typedef struct {
	F32 x;
	F32 y;
	F32 z;
	F32 radius;
} SphereF;

typedef struct {
	U8 red;
	U8 green;
	U8 blue;
	U8 alpha;
} ColorI;

typedef struct {
	F32 red;
	F32 green;
	F32 blue;
	F32 alpha;
} ColorF;

typedef struct {
	U32 size;
	String *names;
	String *values;
} Dictionary;

typedef struct {
	U32 size;
	U8 *data;
} PNG;

typedef struct {
	Point3F origin;
	Point3F direction;
} RayF;

typedef struct {
	Point3F point0;
	Point3F point1;
	Point3F point2;
} TriangleF;

typedef struct {
	Point3F point0;
	Point3F point1;
	Point3F point2;
	Point3F point3;
} TetrahetronF;

#endif
