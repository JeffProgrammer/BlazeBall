//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef mathfns_h
#define mathfns_h

class PlaneF;
class Vec3;

glm::vec2 point3_project_plane(const Vec3 &point, const Vec3 &normal, const Vec3 &origin);
F32 planeF_distance_to_point(const PlaneF &plane, const Vec3 &point);
bool isCollinear(const Vec3 &up, const Vec3 &uq, const Vec3 &vp, const Vec3 &vq);

inline F32 toRadians(F32 degrees) {
	return glm::radians(degrees);
}

#endif
