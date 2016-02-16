//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATH_TRIANGLE_H_
#define _BASE_MATH_TRIANGLE_H_

#include "base/types.h"

class TriangleF {
public:
	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;

	TriangleF() : point0(), point1(), point2() {
		
	}
	TriangleF(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2) : point0(p0), point1(p1), point2(p2) {

	}

	inline const char *toString() const {
		return (std::string("Polygon[") +
		"(" + std::to_string(point0.x) + "," + std::to_string(point0.y) + "," + std::to_string(point0.z) + ")" + ","
		"(" + std::to_string(point1.x) + "," + std::to_string(point1.y) + "," + std::to_string(point1.z) + ")" + ","
		"(" + std::to_string(point2.x) + "," + std::to_string(point2.y) + "," + std::to_string(point2.z) + ")" + "]").c_str();
	}

	inline bool operator==(const TriangleF &other) const {
		U32 same = 0;
		if (other.point0 == point0) same ++;
		if (other.point0 == point1) same ++;
		if (other.point0 == point2) same ++;
		if (other.point1 == point0) same ++;
		if (other.point1 == point1) same ++;
		if (other.point1 == point2) same ++;
		if (other.point2 == point0) same ++;
		if (other.point2 == point1) same ++;
		if (other.point2 == point2) same ++;
		return (same >= 3); // If > 3 then we're technically a line/point, not a triangle
	}

	inline bool isPointOnEdge(const glm::vec3 &point) {
		F32 lineDist01 = glm::length(glm::cross(point0 - point, glm::normalize(point1 - point0)));
		F32 lineDist12 = glm::length(glm::cross(point1 - point, glm::normalize(point2 - point1)));
		F32 lineDist20 = glm::length(glm::cross(point2 - point, glm::normalize(point0 - point2)));

		return (lineDist01 <= 0.001f || lineDist12 <= 0.001f || lineDist20 <= 0.001f);
	}

	inline bool isPointInside(const glm::vec3 &point) {
		//http://www.blackpawn.com/texts/pointinpoly/

		glm::vec3 v0 = point2 - point0,
		          v1 = point1 - point0,
		          v2 = point  - point0;

		F32 dot00 = glm::dot(v0, v0),
		    dot01 = glm::dot(v0, v1),
		    dot02 = glm::dot(v0, v2),
		    dot11 = glm::dot(v1, v1),
		    dot12 = glm::dot(v1, v2);

		F32 invDenom = 1.f / (dot00 * dot11 - dot01 * dot01);
		F32 u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		F32 v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return (u >= 0.f) && (v >= 0.f) && (u + v < 1.f);
	}

	inline bool isTriangleAdjacent(const TriangleF &other) {
		//Check if the two are the same triangle
		if (*this == other)
			return true;

		U32 same = 0;
		if (other.point0 == point0) same ++;
		if (other.point0 == point1) same ++;
		if (other.point0 == point2) same ++;
		if (other.point1 == point0) same ++;
		if (other.point1 == point1) same ++;
		if (other.point1 == point2) same ++;
		if (other.point2 == point0) same ++;
		if (other.point2 == point1) same ++;
		if (other.point2 == point2) same ++;

		//Easy check
		if (same >= 2)
			return true;

		if (isCollinear(point1, point0, other.point1, other.point0))
			return true;
		if (isCollinear(point2, point1, other.point1, other.point0))
			return true;
		if (isCollinear(point0, point2, other.point1, other.point0))
			return true;
		if (isCollinear(point1, point0, other.point2, other.point1))
			return true;
		if (isCollinear(point2, point1, other.point2, other.point1))
			return true;
		if (isCollinear(point0, point2, other.point2, other.point1))
			return true;
		if (isCollinear(point1, point0, other.point0, other.point2))
			return true;
		if (isCollinear(point2, point1, other.point0, other.point2))
			return true;
		if (isCollinear(point0, point2, other.point0, other.point2))
			return true;
		return false;
	}
};

#endif // _BASE_MATH_TRIANGLE_H_