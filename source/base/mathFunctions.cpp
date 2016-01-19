//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include "base/math.h"
#include <glm/gtx/vector_angle.hpp>

#define Sign3d(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

F32 planeF_distance_to_point(const PlaneF &plane, const glm::vec3 &point) {
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z) + plane.d;
}

glm::vec2 point_project_plane(const glm::vec3 &point, glm::vec3 normal, const glm::vec3 &origin) {
	if (glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f)).length() == 0.0f) {
		return glm::vec2(point.x, point.y);
	}
	glm::vec3 xcross = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 ycross = glm::cross(normal, xcross);

	xcross = xcross * (1.0f / xcross.length());
	ycross = ycross * (1.0f / ycross.length());

	glm::vec3 distance = point - origin;
	F32 hypotenuse = distance.length();

	if (hypotenuse == 0.0f)
		return glm::vec2(0, 0);

	F32 theta = glm::angle(distance, xcross);
	//cos theta = adjacent / hypotenuse
	//adjacent = cos theta * hypotenuse
	F32 adjacent = cosf(theta) * hypotenuse;
	F32 opposite = sinf(theta) * hypotenuse;

	return glm::vec2(adjacent, opposite);
}

F32 point_distance_to_line(const glm::vec3 &p, const glm::vec3 &q, const glm::vec3 &point) {
	return (glm::cross(q - p, p - point)).length() / (q - p).length();
}

bool isCollinear(const glm::vec3 &up, const glm::vec3 &uq, const glm::vec3 &vp, const glm::vec3 &vq) {
	//If vp and vq are on line u
	return (point_distance_to_line(up, uq, vp) < 0.1f &&
			point_distance_to_line(up, uq, vq) < 0.1f);
}
