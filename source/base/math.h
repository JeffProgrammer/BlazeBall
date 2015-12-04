//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef math_h
#define math_h

#include "base/types.h"

glm::vec2 point3_project_plane(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 &origin);
F32 planeF_distance_to_point(const PlaneF &plane, const glm::vec3 &point);
bool isCollinear(const glm::vec3 &lineA, const glm::vec3 &lineB);

#endif
