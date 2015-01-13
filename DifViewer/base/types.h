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
#include <stdio.h>
#include <string.h>

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

struct Readable {
public:
	bool read(FILE *file) { return false; };
};

struct Writable {
public:
	bool write(FILE *file) { return false; };
};

struct String : public Readable, Writable {
	U8 *data;
	U32 length;
	bool allocated;

	inline operator const char *() {
		return (const char *)data;
	}
	inline operator char *() {
		return (char *)data;
	}
	inline bool operator==(String *str) {
		return strcmp((const char *)data, (const char *)str->data) == 0;
	}
	inline bool operator==(String str) {
		return strcmp((const char *)data, (const char *)str.data) == 0;
	}
	inline bool operator!=(String *str) {
		return !operator==(str);
	}
	inline bool operator!=(String str) {
		return !operator==(str);
	}
	String() : data(nullptr), length(0) {
		allocated = false;
	}
	String(U32 length) : data(new U8[length]), length(length) {
		allocated = true;
	}
	~String() {
//		if (allocated)
//			delete data;
	}
	String(const char *bytes) : data(new U8[(U8)strlen(bytes) + 1]), length((U8)strlen(bytes) + 1) {
		memcpy(data, bytes, length);
		allocated = true;
	}
	String(U8 *bytes, U32 length) : data(new U8[length]), length(length) {
		memcpy(data, bytes, length);
		allocated = true;
	}
	String(String *other) : data(new U8[other->length + 1]), length(other->length + 1) {
		memcpy(data, other->data, length - 1);
		data[length - 1] = 0;
		allocated = true;
	}

	bool read(FILE *file);
	bool write(FILE *file);
};

#include "point2.h"
#include "point3.h"
#include "point4.h"
#include "color.h"

typedef Point2<U16> Point2I;
typedef Point2<F32> Point2F;
typedef Point3<U32> Point3I;
typedef Point3<F32> Point3F;
typedef Point3<F64> Point3D;

typedef Color<U8>  ColorI;
typedef Color<F32> ColorF;

//More names stolen from TGE

class QuatF : public Readable, Writable {
public:
	F32 w;
	F32 x;
	F32 y;
	F32 z;

	bool read(FILE *file);
	bool write(FILE *file);
};

class PlaneF : public Readable, Writable {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 d;

	bool read(FILE *file);
	bool write(FILE *file);
};

class BoxF : public Readable, Writable {
public:
	F32 minX;
	F32 minY;
	F32 minZ;
	F32 maxX;
	F32 maxY;
	F32 maxZ;

	inline Point3F getMin() {
		return Point3F(minX, minY, minZ);
	}
	inline Point3F getMax() {
		return Point3F(maxX, maxY, maxZ);
	}
	inline Point3F getCenter() {
		return (getMax() + getMin()) / 2;
	}

	bool read(FILE *file);
	bool write(FILE *file);
};

class SphereF : public Readable, Writable {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 radius;

	bool read(FILE *file);
	bool write(FILE *file);
};

class Dictionary : public Readable, Writable {
public:
	U32 size;
	String **names;
	String **values;

	bool read(FILE *file);
	bool write(FILE *file);

	String *get(String *key) {
		for (U32 i = 0; i < size; i ++) {
			if (names[i] == key)
				return values[i];
		}
		return nullptr;
	}
};

class PNG : public Readable, Writable {
public:
	U32 size;
	U8 *data;

	bool read(FILE *file);
	bool write(FILE *file);
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
