//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef mathfns_h
#define mathfns_h

class PlaneF;

glm::vec2 point3_project_plane(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 &origin);
F32 planeF_distance_to_point(const PlaneF &plane, const glm::vec3 &point);
bool isCollinear(const glm::vec3 &up, const glm::vec3 &uq, const glm::vec3 &vp, const glm::vec3 &vq);

inline F32 toRadians(F32 degrees) {
	return glm::radians(degrees);
}

#endif
