//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATHTYPES_H_
#define _BASE_MATHTYPES_H_

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

	BoxF() {
		minX = 0.0f;
		minY = 0.0f;
		minZ = 0.0f;
		maxX = 0.0f;
		maxY = 0.0f;
		maxZ = 0.0f;
	}

	inline Vec3 getMin() const {
		return Vec3(minX, minY, minZ);
	}
	inline Vec3 getMax() const {
		return Vec3(maxX, maxY, maxZ);
	}
	inline Vec3 getCenter() const {
		return (getMax() + getMin()) / 2.0f;
	}
};

class TetrahetronF {
public:
	Vec3 point0;
	Vec3 point1;
	Vec3 point2;
	Vec3 point3;
};

struct Vertex {
	Vec3 point;
	glm::vec2 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec3 bitangent;
};

struct Triangle {
	Vertex verts[3];
};

struct Point {
	Vec3 vertex;
	Vec3 color;

	Point(const Vec3 &vertex, const Vec3 &color) : vertex(vertex), color(color) {

	}
};

#endif