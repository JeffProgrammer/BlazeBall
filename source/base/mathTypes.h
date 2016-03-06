//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
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

	BoxF(const glm::vec3 &min, const glm::vec3 &max) {
		minX = min.x;
		minY = min.y;
		minZ = min.z;
		maxX = max.x;
		maxY = max.y;
		maxZ = max.z;
	}

	inline glm::vec3 getMin() const {
		return glm::vec3(minX, minY, minZ);
	}
	inline glm::vec3 getMax() const {
		return glm::vec3(maxX, maxY, maxZ);
	}
	inline glm::vec3 getCenter() const {
		return (getMax() + getMin()) / 2.0f;
	}

	inline bool intersectsBox(const BoxF &box) const {
		if (box.minX > maxX)
			return false;
		if (box.minY > maxY)
			return false;
		if (box.minZ > maxZ)
			return false;
		if (box.maxX < minX)
			return false;
		if (box.maxY < minY)
			return false;
		if (box.maxZ < minZ)
			return false;
		return true;
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

struct Point {
	glm::vec3 vertex;
	glm::vec3 color;

	Point(const glm::vec3 &vertex, const glm::vec3 &color) : vertex(vertex), color(color) {

	}
};

#endif // _BASE_MATHTYPES_H_