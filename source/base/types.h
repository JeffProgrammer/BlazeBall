//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
#include <glm/gtc/constants.hpp>

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

class PlaneF {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 d;

	PlaneF(const DIF::PlaneF &plane) : x(plane.x), y(plane.y), z(plane.z), d(plane.d) {}
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

class TetrahetronF {
public:
	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
};

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
