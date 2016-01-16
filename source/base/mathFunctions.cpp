//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include "base/math.h"

#define Sign3d(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

F32 planeF_distance_to_point(const PlaneF &plane, const Vec3 &point) {
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z) + plane.d;
}

glm::vec2 point_project_plane(const Vec3 &point, Vec3 normal, const Vec3 &origin) {
	if (Vec3::cross(normal, Vec3(0.0f, 0.0f, 1.0f)).length() == 0.0f) {
		return glm::vec2(point.x, point.y);
	}
	Vec3 xcross = Vec3::cross(normal, Vec3(0.0f, 0.0f, 1.0f));
	Vec3 ycross = Vec3::cross(normal, xcross);

	xcross = xcross * (1.0f / xcross.length());
	ycross = ycross * (1.0f / ycross.length());

	Vec3 distance = point - origin;
	F32 hypotenuse = distance.length();

	if (hypotenuse == 0.0f)
		return glm::vec2(0, 0);

	F32 theta = Vec3::angle(distance, xcross);
	//cos theta = adjacent / hypotenuse
	//adjacent = cos theta * hypotenuse
	F32 adjacent = cosf(theta) * hypotenuse;
	F32 opposite = sinf(theta) * hypotenuse;

	return glm::vec2(adjacent, opposite);
}

F32 point_distance_to_line(const Vec3 &p, const Vec3 &q, const glm::vec3 &point) {
	return	(Vec3::cross(q - p, p - point)).length() / (q - p).length();
}

bool isCollinear(const Vec3 &up, const Vec3 &uq, const Vec3 &vp, const Vec3 &vq) {
	//If vp and vq are on line u
	return (point_distance_to_line(up, uq, vp) < 0.1f &&
			point_distance_to_line(up, uq, vq) < 0.1f);
}
