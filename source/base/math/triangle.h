#ifndef triangle_h
#define triangle_h

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

	inline bool isPointOnEdge(const glm::vec3 &point) {
		F32 lineDist01 = glm::length(glm::cross(point0 - point, glm::normalize(point1 - point0)));
		F32 lineDist12 = glm::length(glm::cross(point1 - point, glm::normalize(point2 - point1)));
		F32 lineDist20 = glm::length(glm::cross(point2 - point, glm::normalize(point0 - point2)));

		return (lineDist01 == 0 || lineDist12 == 0 || lineDist20 == 0);
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
};

#endif
