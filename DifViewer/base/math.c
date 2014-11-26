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

#include <stdio.h>
#include <math.h>
#include "math.h"

#define Sign3d(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

F32 planeF_distance_to_point(PlaneF plane, Point3F point) {
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z) + plane.d;
}

Point3F point3F_add_point3F(Point3F point0, Point3F point1) {
	return (Point3F){point0.x + point1.x, point0.y + point1.y, point0.z + point1.z};
}

Point3F point3F_subtract_point3F(Point3F point0, Point3F point1) {
	return (Point3F){point0.x - point1.x, point0.y - point1.y, point0.z - point1.z};
}

Point3F point3F_scale(Point3F point0, F32 scalar) {
	return (Point3F){point0.x * scalar, point0.y * scalar, point0.z * scalar};
}

F32 point3F_dot_point3F(Point3F point0, Point3F point1) {
	return point0.x * point1.x + point0.y * point1.y + point0.z * point1.z;
}

Point3F point3F_cross_point3F(Point3F point0, Point3F point1) {
	return (Point3F){(point0.y * point1.z) - (point0.z * point1.y),
	                 (point0.z * point1.x) - (point0.x * point1.z),
	                 (point0.x * point1.y) - (point0.y * point1.x)};
}

Point3F point3F_convert_to_torque(Point3F point) {
	return (Point3F){point.x, point.z, point.y};
}

Point3F point3F_convert_from_torque(Point3F point) {
	return (Point3F){point.x, point.z, point.y};
}

F32 point3F_distance_to_point3F(Point3F point0, Point3F point1) {
	return sqrt((point0.x * point1.x) + (point0.y * point1.y) + (point0.z * point1.z));
}

bool rayF_intersects_planeF(RayF ray, PlaneF plane) {
	//http://antongerdelan.net/opengl/raycasting.html
	//t = (RayOrigin • PlaneNormal + PlaneOffset) / (RayDirection • PlaneNormal)
	//miss if denom == 0
	//intersect if t > 0

	Point3F normal;
	normal.x = plane.x;
	normal.y = plane.y;
	normal.z = plane.z;

	F32 denominator = point3F_dot_point3F(ray.direction, normal);
	if (denominator == 0) {
		return false;
	}

	F32 numerator = (point3F_dot_point3F(ray.origin, normal) + plane.d);

	if (-numerator / denominator > 0) {
		return true;
	} else {
		return false;
	}
}

Point3F rayF_planeF_intersection(RayF ray, PlaneF plane) {
	//http://antongerdelan.net/opengl/raycasting.html
	//t = (RayOrigin • PlaneNormal + PlaneOffset) / (RayDirection • PlaneNormal)
	//miss if denom == 0
	//intersect if t > 0

	Point3F normal;
	normal.x = plane.x;
	normal.y = plane.y;
	normal.z = plane.z;

	F32 denominator = point3F_dot_point3F(ray.direction, normal);
	if (denominator == 0) {
		return (Point3F){INT32_MIN, INT32_MIN, INT32_MIN};
	}

	F32 numerator = (point3F_dot_point3F(ray.origin, normal) + plane.d);

	if (-numerator / denominator > 0) {
		//xyz = RayOrigin + (RayNormal * t)
		return point3F_add_point3F(ray.origin, point3F_scale(ray.direction, -numerator / denominator));
	} else {
		return (Point3F){INT32_MIN, INT32_MIN, INT32_MIN};
	}
}

Point3F point3F_sub_point3F(Point3F point0, Point3F point1) {
	return point3F_add_point3F(point0, point3F_scale(point1, -1.0f));
}

Point3F point3F_proj_point3F(Point3F point0, Point3F point1) {
	return point3F_scale(point1, point3F_dot_point3F(point0, point1) / point3F_dot_point3F(point1, point1));
}

Point3F point3F_rej_point3F(Point3F point0, Point3F point1) {
	return point3F_sub_point3F(point0, point3F_proj_point3F(point0, point1));
}

F32 point3F_len(Point3F point0) {
	return point0.x*point0.x + point0.y*point0.y + point0.z*point0.z;
}

F32 rayF_intersects_triangle(RayF ray, TriangleF triangle) {
	Point3F ab = point3F_sub_point3F(triangle.point1, triangle.point0);
	Point3F ac = point3F_sub_point3F(triangle.point2, triangle.point0);
	Point3F nor = point3F_cross_point3F(ab, ac);
	Point3F oa = point3F_sub_point3F(triangle.point0, ray.origin);
	F32 num = point3F_dot_point3F(nor, oa);
	F32 denom = point3F_dot_point3F(nor, ray.direction);
	if (denom == 0.0f)
		return -1.0f;
	F32 r = num/denom;
	if (r < 0.0f)
		return -1.0f;
	Point3F p = point3F_add_point3F(ray.origin, point3F_scale(ray.direction, r));
	Point3F ap = point3F_sub_point3F(p, triangle.point0);
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
	else if (point3F_len(point3F_cross_point3F(ab,ac)) == 0.0f) {
		return -1.0f;
	}
	else {
		fprintf(stderr, "WTF IS UP WITH THIS TRIANGLE?\n%f %f %f\n%f %f %f\n%f %f %f\n", ab.x, ab.y, ab.z, ac.x, ac.y, ac.z, ap.x, ap.y, ap.z);
	}
	F32 kld = kc1*lc2 - kc2*lc1;
	F32 kn = c1*lc2 - c2*lc1;
	F32 ln = kc1*c2 - kc2*c1;
	F32 k = kn/kld;
	F32 l = ln/kld;
	if (k >= 0.0f && l >= 0.0f && k + l <= 1.0f)
		return point3F_len(point3F_sub_point3F(p, ray.origin));
	return -1.0f;
}