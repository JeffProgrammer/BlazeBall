//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// Copyright (c) 2015 Whirligig231
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

#ifndef ray_h
#define ray_h

#include "base/types.h"

template <typename T>
class Ray {
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(const glm::vec3 &origin, const glm::vec3 &direction) : origin(origin), direction(direction) {};
	Ray(const T &ox, const T &oy, const T &oz, const T &dx, const T &dy, const T &dz) : origin(glm::vec3(ox, oy, oz)), direction(glm::vec3(dx, dy, dz)) {};

	bool intersects(const PlaneF &plane) const;
	bool intersects(const TriangleF &triangle) const;

	T distance(const TriangleF &triangle) const;
	glm::vec3 intersection(const PlaneF &plane) const;
};

typedef Ray<F32> RayF;

template <typename T>
bool Ray<T>::intersects(const PlaneF &plane) const {
	//http://antongerdelan.net/opengl/raycasting.html
	//t = (RayOrigin • PlaneNormal + PlaneOffset) / (RayDirection • PlaneNormal)
	//miss if denom == 0
	//intersect if t > 0

	glm::vec3 normal(plane.x, plane.y, plane.z);

	F32 denominator = glm::dot(direction, normal);
	if (denominator == 0) {
		return false;
	}

	F32 numerator = (glm::dot(origin, normal) + plane.d);

	if (-numerator / denominator > 0) {
		return true;
	} else {
		return false;
	}
}

template <typename T>
glm::vec3 Ray<T>::intersection(const PlaneF &plane) const {
	//http://antongerdelan.net/opengl/raycasting.html
	//t = (RayOrigin • PlaneNormal + PlaneOffset) / (RayDirection • PlaneNormal)
	//miss if denom == 0
	//intersect if t > 0

	glm::vec3 normal(plane.x, plane.y, plane.z);

	F32 denominator = glm::dot(direction, normal);
	if (denominator == 0) {
		return glm::vec3(-0x80000000, -0x80000000, -0x80000000);
	}

	F32 numerator = (glm::dot(origin, normal) + plane.d);

	if (-numerator / denominator > 0) {
		//xyz = RayOrigin + (RayNormal * t)
		return origin + direction * (-numerator / denominator);
	} else {
		return glm::vec3(-0x80000000, -0x80000000, -0x80000000);
	}
}

template<typename T>
T Ray<T>::distance(const TriangleF &triangle) const {
	//	triangle.point0 = triangle.point0.roundThousands();
	//	triangle.point1 = triangle.point1.roundThousands();
	//	triangle.point2 = triangle.point2.roundThousands();

	glm::vec3 ab = triangle.point1 - triangle.point0;
	glm::vec3 ac = triangle.point2 - triangle.point0;
	glm::vec3 nor = glm::cross(ab, ac);
	glm::vec3 oa = triangle.point0 - origin;
	F32 num = glm::dot(nor, oa);
	F32 denom = glm::dot(nor, direction);
	if (denom == 0.0f)
		return -1.0f;
	F32 r = num/denom;
	if (r < 0.0f)
		return -1.0f;
	glm::vec3 p = origin + direction * r;
	glm::vec3 ap = p - triangle.point0;
	bool canXY = true, canYZ = true, canZX = true;
	if (ab.x*ac.y - ab.y*ac.x == 0.0f)
		canXY = false;
	if (ab.x*ac.z - ab.z*ac.x == 0.0f)
		canZX = false;
	if (ab.y*ac.z - ab.z*ac.y == 0.0f)
		canYZ = false;
	F32 kc1, lc1, kc2, lc2, c1, c2;
	if (canXY) {
		kc1 = ab.x;
		kc2 = ab.y;
		lc1 = ac.x;
		lc2 = ac.y;
		c1 = ap.x;
		c2 = ap.y;
	}
	else if (canZX) {
		kc1 = ab.x;
		kc2 = ab.z;
		lc1 = ac.x;
		lc2 = ac.z;
		c1 = ap.x;
		c2 = ap.z;
	}
	else if (canYZ) {
		kc1 = ab.z;
		kc2 = ab.y;
		lc1 = ac.z;
		lc2 = ac.y;
		c1 = ap.z;
		c2 = ap.y;
	}
	else if (glm::cross(ab, ac).length() == 0.0f) {
		return -1.0f;
	}
	else {
		fprintf(stderr, "Collinear triangle?\n%f %f %f\n%f %f %f\n%f %f %f\n", ab.x, ab.y, ab.z, ac.x, ac.y, ac.z, ap.x, ap.y, ap.z);
		return -1.0f;
	}
	F32 kld = kc1*lc2 - kc2*lc1;
	F32 kn = c1*lc2 - c2*lc1;
	F32 ln = kc1*c2 - kc2*c1;
	F32 k = kn/kld;
	F32 l = ln/kld;
	if (k >= 0.0f && l >= 0.0f && k + l <= 1.0f)
		return glm::distance(p, origin);
	return -1.0f;
}

template <typename T>
bool Ray<T>::intersects(const TriangleF &triangle) const {
	return distance(triangle) > 0;
}

#endif
