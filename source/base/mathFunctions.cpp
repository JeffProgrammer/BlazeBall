//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <glm/gtx/vector_angle.hpp>
#include "base/math.h"

#define Sign3d(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

F32 planeF_distance_to_point(const PlaneF &plane, const Vec3 &point) {
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z) + plane.d;
}

glm::vec2 point_project_plane(const glm::vec3 &point, Vec3 normal, const Vec3 &origin) {
	if (normal.cross(Vec3(0.0f, 0.0f, 1.0f)).length() == 0.0f) {
		return glm::vec2(point.x, point.y);
	}
	Vec3 xcross = normal.cross(Vec3(0.0f, 0.0f, 1.0f));
	Vec3 ycross = normal.cross(xcross);

	xcross = xcross * (1.0f / xcross.length());
	ycross = ycross * (1.0f / ycross.length());

	Vec3 distance = point - origin;
	F32 hypotenuse = distance.length();

	if (hypotenuse == 0)
		return glm::vec2(0, 0);

	F32 theta = glm::angle(static_cast<glm::vec3>(distance), xcross);
	//cos theta = adjacent / hypotenuse
	//adjacent = cos theta * hypotenuse
	F32 adjacent = cosf(theta) * hypotenuse;
	F32 opposite = sinf(theta) * hypotenuse;

	return glm::vec2(adjacent, opposite);
}

F32 point_distance_to_line(const Vec3 &p, const Vec3 &q, const glm::vec3 &point) {
	return glm::length(glm::cross(q - p, p - point)) / glm::length(q - p);
}

bool isCollinear(const Vec3 &up, const Vec3 &uq, const Vec3 &vp, const Vec3 &vq) {
	//If vp and vq are on line u
	return (point_distance_to_line(up, uq, vp) < 0.1f &&
			point_distance_to_line(up, uq, vq) < 0.1f);
}
