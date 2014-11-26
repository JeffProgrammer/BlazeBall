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
	return (Point3F){(point0.y * point1.z) - (point0.z - point1.y),
	                 (point0.z * point1.x) - (point0.x - point1.z),
	                 (point0.x * point1.y) - (point0.y - point1.x)};
}

Point3F point3F_convert_to_torque(Point3F point) {
	return (Point3F){point.x, point.z, point.y};
}

Point3F point3F_convert_from_torque(Point3F point) {
	return (Point3F){point.x, point.z, point.y};
}

F32 tetrahedronF_area(TetrahetronF tetrahedron) {
	//Tetrahedron volume:
	//V = ((a-d) dot ((b - d) cross (c - d))) / 6

	return (point3F_dot_point3F(point3F_subtract_point3F(tetrahedron.point0, tetrahedron.point3), point3F_cross_point3F(point3F_subtract_point3F(tetrahedron.point1, tetrahedron.point3), point3F_subtract_point3F(tetrahedron.point2, tetrahedron.point3)))) / 6;
}

F32 tetrahedronF_signed_volume(TetrahetronF tetrahedron) {
	//Tetrahedron signed volume:
	//
	// 1   [xa - xd, ya - yd, za - zd]
	// - * [xb - xd, yb - yd, zb - zd]
	// 6   [xc - xd, yc - yd, zc - zd]
	//

	F32 a = tetrahedron.point0.x - tetrahedron.point3.x;
	F32 b = tetrahedron.point0.y - tetrahedron.point3.y;
	F32 c = tetrahedron.point0.z - tetrahedron.point3.z;
	F32 d = tetrahedron.point1.x - tetrahedron.point3.x;
	F32 e = tetrahedron.point1.y - tetrahedron.point3.y;
	F32 f = tetrahedron.point1.z - tetrahedron.point3.z;
	F32 g = tetrahedron.point2.x - tetrahedron.point3.x;
	F32 h = tetrahedron.point2.y - tetrahedron.point3.y;
	F32 i = tetrahedron.point2.z - tetrahedron.point3.z;

	//(aei + bfg + cdh) - (ceg + bdi + afh)

	return ((a*e*i)+(b*f*g)*(c*d*h))-((c*e*g)+(b*d*i)+(a*f*h));
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

IntersectionType rayF_intersects_triangle(RayF ray, TriangleF triangle) {
	//Intersection algorithm from
	//http://147.228.63.9/wscg2001/papers_2001/r75.pdf
	//Rafael J. Segura, Francisco R. Feito

	Point3F q = ray.origin;
	Point3F qp = point3F_add_point3F(ray.origin, point3F_scale(ray.direction, 100));
	Point3F a = triangle.point0;
	Point3F b = triangle.point1;
	Point3F c = triangle.point2;

	F32 i = Sign3d(tetrahedronF_signed_volume((TetrahetronF){q, qp, a, c}));
	F32 j = Sign3d(tetrahedronF_signed_volume((TetrahetronF){q, qp, b, c}));
	F32 k = Sign3d(tetrahedronF_signed_volume((TetrahetronF){q, qp, a, b}));

	if ((i == 0 && j == 0) ||
		(j == 0 && k == 0) ||
		(k == 0 && i == 0)) {
		return IntersectionTypeVertex;
	}
	if ((i == 0) && (i == k))
		return IntersectionTypeAC;
	if ((j == 0) && (i == k))
		return IntersectionTypeBC;
	if ((k == 0) && (i == j))
		return IntersectionTypeAB;
	if ((i == j) && (j == k))
		return IntersectionTypeInside;
	return IntersectionTypeOutside;
}
