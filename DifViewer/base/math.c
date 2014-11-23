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

F32 planeF_distance_to_point(PlaneF plane, Point3F point) {
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z) + plane.d;
}

Point3F point3F_add_point3F(Point3F point0, Point3F point1) {
	return (Point3F){point0.x + point1.x, point0.y + point1.y, point0.z + point1.z};
}

Point3F point3F_scale(Point3F point0, F32 scalar) {
	return (Point3F){point0.x * scalar, point0.y * scalar, point0.z * scalar};
}

F32 point3F_dot_point3F(Point3F point0, Point3F point1) {
	return point0.x * point1.x + point0.y * point1.y + point0.z * point1.z;
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

S32 pnpoly(U32 nvert, Point2F *vert, Point2F test)
{
	int i, j, c = 0;
	for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((vert[i].y>test.y) != (vert[j].y>test.y)) &&
			(test.x < (vert[j].x-vert[i].x) * (test.y-vert[i].y) / (vert[j].y-vert[i].y) + vert[i].x) )
			c = !c;
	}
	return c;
}