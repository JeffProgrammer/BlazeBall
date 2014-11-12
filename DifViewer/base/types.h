//------------------------------------------------------------------------------
// Copyright (C) 2014 Glenn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
typedef U8 * PNG; //unused

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
	U32 index1;
	F32 distance1;
	U32 index2;
	F32 distance2;
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
	Point3F point0;
	Point3F point1;
	Point3F point2;

	Point3F normal;

	ColorF color;
} Triangle;

#endif
