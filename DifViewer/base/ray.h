//
//  ray.h
//  DifViewer
//
//  Created by Glenn Smith on 1/2/15.
//  Copyright (c) 2015 CouleeApps. All rights reserved.
//

#ifndef ray_h
#define ray_h

#include "point3.h"

template <typename T>
class Ray {
public:
	Point3<T> origin;
	Point3<T> direction;

	Ray(const Point3<T> &origin, const Point3<T> &direction) : origin(origin), direction(direction) {};
	Ray(T ox, T oy, T oz, T dx, T dy, T dz) : origin(Point3<T>(ox, oy, oz)), direction(Point3<T>(dx, dy, dz)) {};

	bool intersects(PlaneF plane);
	bool intersects(TriangleF triangle);

	F32 distance(TriangleF triangle);
	Point3<T> intersection(PlaneF plane);
};

#endif
