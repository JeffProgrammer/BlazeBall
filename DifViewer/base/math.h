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

#ifndef math_h
#define math_h

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

Point3F point3F_add_point3F(Point3F point0, Point3F point1);
Point3F point3F_subtract_point3F(Point3F point0, Point3F point1);
F32 point3F_dot_point3F(Point3F point0, Point3F point1);
Point3F point3F_scale(Point3F point0, F32 scalar);
F32 point3F_distance_to_point3F(Point3F point0, Point3F point1);
Point3F point3F_cross_point3F(Point3F point0, Point3F point1);
Point3F point3F_proj_point3F(Point3F point0, Point3F point1);
Point3F point3F_rej_point3F(Point3F point0, Point3F point1);

Point3F point3F_convert_to_torque(Point3F point);
Point3F point3F_convert_from_torque(Point3F point);

F32 tetrahedronF_area(TetrahetronF tetrahedron);

F32 planeF_distance_to_point(PlaneF plane, Point3F point);
bool rayF_intersects_planeF(RayF ray, PlaneF plane);
Point3F rayF_planeF_intersection(RayF ray, PlaneF plane);

F32 rayF_intersects_triangle(RayF ray, TriangleF triangle);

#ifdef __cplusplus
}
#endif

#endif
